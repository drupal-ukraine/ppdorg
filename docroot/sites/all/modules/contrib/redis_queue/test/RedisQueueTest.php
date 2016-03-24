<?php
$path = dirname(__FILE__);
require($path . '/system.queue.inc');
require($path . '/redis_queue_tests.inc');
require($path . '/../redis.queue.inc');

class RedisQueueTest extends PHPUnit_Framework_TestCase {
  public $q;
  public $name;

  public function __construct() {
    $this->name = 'redis-queue-test-' . time();
  }

  public function setUp() {
    $this->q = new RedisQueue($this->name);
  }

  /**
   * @test
   */
  public function create() {
    $res = $this->q->createItem('test-queue-item-create');
    $num = $this->q->numberOfItems();
    $this->assertEquals(1, $num);
  }

  /**
   * @test
   */
  public function claim() {
    $data = 'test-queue-item-claimed';
    $res = $this->q->createItem($data);
    $item = $this->q->claimItem();
    $this->assertEquals($data, $item->data);
  }

  /**
   * @test
   */
  public function claimBlocking() {
    $data = 'test-queue-item-claimed';
    $res = $this->q->createItem($data);
    $item = $this->q->claimItemBlocking(10);
    $this->assertEquals($data, $item->data);
  }

  /**
   * @test
   */
  public function release() {
    $data = 'test-queue-item';

    $res = $this->q->createItem($data);
    $item = $this->q->claimItem();

    $num = $this->q->numberOfItems();
    $this->assertEquals(0, $num);

    $res = $this->q->releaseItem($item);

    $num = $this->q->numberOfItems();
    $this->assertEquals(1, $num);
  }

  /**
   * @test
   */
  public function order() {
    $keys = array('test1', 'test2', 'test3');
    foreach ($keys as $k) {
      $this->q->createItem($k);
    }

    $first = $this->q->claimItem();
    $this->assertEquals($first->data, $keys[0]);

    $second = $this->q->claimItem();
    $this->assertEquals($second->data, $keys[1]);

    $this->q->releaseItem($first);

    $third = $this->q->claimItem();
    $this->assertEquals($third->data, $keys[2]);

    $first_again = $this->q->claimItem();
    $this->assertEquals($first_again->data, $keys[0]);

    $num = $this->q->numberOfItems();
    $this->assertEquals(0, $num);
  }

  /**
   * @test
   */
  public function lease() {
    $data = 'test-queue-item';
    $res = $this->q->createItem($data);
    $num = $this->q->numberOfItems();
    $this->assertEquals(1, $num);
    $item = $this->q->claimItem(1);
    // In Redis 2.4 the expire could be between zero to one seconds off. 
    sleep(2);
    $expired = $this->q->expire();
    $this->assertEquals(1, $expired);
    $this->assertEquals(1, $this->q->numberOfItems());
    // Create a second queue to test expireAll()
    $q2 = new RedisQueue($this->name . '_2');
    $q2->createItem($data);
    $q2->createItem($data);
    $this->assertEquals(2, $q2->numberOfItems());
    $item = $this->q->claimItem(1);
    $item2 = $q2->claimItem(1);
    $this->assertEquals(1, $q2->numberOfItems());
    sleep(2);
    $expired = $this->q->expireAll();
    $this->assertEquals(2, $expired);
    $this->assertEquals(1, $this->q->numberOfItems());
    $this->assertEquals(2, $q2->numberOfItems());
    $q2->deleteQueue();
  }

  /**
   * Delete all keys before each test.
   */
  public function teardown() {
    $this->q->deleteQueue();
  }
}
