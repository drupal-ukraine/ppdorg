<?php

class RedisQueue implements DrupalQueueInterface {
  // The name of the queue that holds available items.
  protected $avail;
  // The queue that holds claimed items.
  protected $claimed;
  // The PhpRedis object which connects to the redis server.
  protected $redis;

  protected $reserve_timeout;

  protected $name;

  /**
   * Start working with a queue.
   *
   * @param $name
   *   Arbitrary string. The name of the queue to work with.
   */
  public function __construct($name) {
    $this->name = $name;
    $this->avail = 'drupal:queue:' . $name . ':avail';
    $this->claimed = 'drupal:queue:' . $name . ':claimed';
    $this->lease = $this->claimed . '_lease:';
    $this->redis = new Redis();
    $options = self::getOptions($name);
    $this->reserve_timeout = $options['reserve_timeout'];
    $this->redis->connect($options['host'], $options['port']);
  }

  static function getOptions($name) {
    $options = variable_get('redis_queue_' . $name, array());
    $defaults = variable_get('redis_default_queue', array()) + array(
      'host' => variable_get('redis_client_host', '127.0.0.1'),
      'port' => variable_get('redis_client_port', 6379),
      'reserve_timeout' => NULL,
    );
    $options += $defaults;

    return $options;
  }

  /**
   * Add a queue item and store it directly to the queue.
   *
   * @param $data
   *   Arbitrary data to be associated with the new task in the queue.
   * @return
   *   TRUE if the item was successfully created and was (best effort) added
   *   to the queue, otherwise FALSE. We don't guarantee the item was
   *   committed to disk, that your disk wasn't hit by a meteor, etc, but as
   *   far as we know, the item is now in the queue.
   */
  public function createItem($data) {
    $record = new stdClass();
    $record->data = $data;
    $record->qid = $this->incrementId();
    // We cannot rely on REQUEST_TIME because many items might be created
    // by a single request which takes longer than 1 second.
    $record->timestamp = time();
    $result = $this->redis->multi()
                          ->hsetnx($this->avail . '_hash', $record->qid, serialize($record))
                          ->llen($this->avail)
                          ->lpush($this->avail, $record->qid)->exec();
    return $result[0] && $result[2] > $result[1];
  }

  protected function incrementId() {
    return $this->redis->incr($this->avail . '_counter');
  }

  /**
   * Retrieve the number of items in the queue.
   *
   * This is intended to provide a "best guess" count of the number of items in
   * the queue. Depending on the implementation and the setup, the accuracy of
   * the results of this function may vary.
   *
   * e.g. On a busy system with a large number of consumers and items, the
   * result might only be valid for a fraction of a second and not provide an
   * accurate representation.
   *
   * @return
   *   An integer estimate of the number of items in the queue.
   */
  public function numberOfItems() {
    return $this->redis->lLen($this->avail);
  }

  /**
   * Claim an item in the queue for processing.
   *
   * @param $lease_time
   *   How long the processing is expected to take in seconds, defaults to an
   *   hour. After this lease expires, the item will be reset and another
   *   consumer can claim the item. For idempotent tasks (which can be run
   *   multiple times without side effects), shorter lease times would result
   *   in lower latency in case a consumer fails. For tasks that should not be
   *   run more than once (non-idempotent), a larger lease time will make it
   *   more rare for a given task to run multiple times in cases of failure,
   *   at the cost of higher latency.
   * @return
   *   On success we return an item object. If the queue is unable to claim an
   *   item it returns false. This implies a best effort to retrieve an item
   *   and either the queue is empty or there is some other non-recoverable
   *   problem.
   */
  public function claimItem($lease_time = 30) {
    $item = FALSE;
    if (is_numeric($this->reserve_timeout)) {
      $item = $this->claimItemBlocking($lease_time);
    }
    else {
      $qid = $this->redis->rpoplpush($this->avail, $this->claimed);
      if ($qid) {
        $job = $this->redis->hget($this->avail . '_hash', $qid);
        if ($job) {
          $item = unserialize($job);
          $this->redis->setex($this->lease . $item->qid, $lease_time, '1');
        }
      }
    }
    return $item;
  }

  /**
   * A blocking version of claimItem to be used with long-running queue workers
   * like waiting_queue.
   */
  public function claimItemBlocking($lease_time) {
    $item = FALSE;
    $qid = $this->redis->brpoplpush($this->avail, $this->claimed, $this->reserve_timeout);
    if ($qid) {
      $job = $this->redis->hget($this->avail . '_hash', $qid);
      if ($job) {
        $item = unserialize($job);
        $this->redis->setex($this->lease . $item->qid, $lease_time, '1');
      }
    }
    return $item;
  }

  /**
   * Delete a finished item from the queue.
   *
   * @param $item
   *   The item returned by DrupalQueueInterface::claimItem().
   */
  public function deleteItem($item) {
    $this->redis->multi()
                ->lrem($this->claimed, $item->qid, -1)
                ->hdel($this->avail . '_hash', $item->qid)->exec();
  }

  public function createQueue() {}

  /**
   * Delete a queue and every item in the queue.
   */
  public function deleteQueue() {
    $this->redis->del($this->claimed, $this->avail, $this->avail . '_hash', $this->avail . '_counter');
    foreach ($this->redis->keys($this->lease . '*') as $key) {
      $this->redis->del($key);
    }
  }

  public function releaseItem($item) {
    $this->redis->multi()
                ->lrem($this->claimed, $item->qid, -1)
                ->lpush($this->avail, $item->qid)->exec();
  }

  /**
   * Expire claims in this queue.
   *
   * @return number of items returned to available.
   */
  public function expire() {
    return $this->_expireArbitrary($this->claimed, $this->lease, $this->avail);
  }

  /**
   * Helper function to expire claims using arbitrary queue keys.
   *
   * @return number of items returned to available.
   */
  protected function _expireArbitrary($claimed, $lease, $avail) {
    $expired = 0;
    foreach ($this->redis->lrange($claimed, 0, -1) as $qid) {
      if (!$this->redis->exists($lease . $qid)) {
        // The lease expired for this ID.
        $this->redis->multi()
                    ->lrem($claimed, $qid, -1)
                    ->lpush($avail, $qid)->exec();
        $expired++;
      }
    }
    return $expired;
  }

  /**
   * Expire claims in all queues.
   *
   * @return number of items returned to available.
   */
  public function expireAll() {
    $expired = 0;
    foreach ($this->redis->keys("drupal:queue:*:claimed") as $claimed) {
      $lease = $claimed . '_lease:';
      $avail = preg_replace('/^(drupal:queue:.*):claimed$/', '$1:avail', $claimed);
      $expired += $this->_expireArbitrary($claimed, $lease, $avail);
    }
    return $expired;
  }

  /**
   * Dumps items in the queue.
   *
   * @return
   *   An array of queue items.
   */
  public function dump() {
    return $this->redis->hgetall($this->avail . '_hash');
  }

  /**
   * Dump currently claimed queue items.
   *
   * @return
   *   An array of queue IDs.
   */
  public function dumpClaimed() {
    return $this->redis->lrange($this->claimed, 0, -1);
  }
}
