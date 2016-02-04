<?php
/**
 * @file
 * API requests.
 */

/**
 * Class APICall.
 */
class APICall {

  /**
   * @var string
   */
  private $url = '';
  /**
   * @var array
   */
  private $query = array();
  /**
   * @var stdClass
   */
  private $result;

  /**
   * APICall constructor.
   *
   * @param string $base
   *   An URL with API.
   */
  public function __construct($base = 'https://www.drupal.org') {
    $this->url = rtrim($base, '/');
  }

  /**
   * Specify resource of the API.
   *
   * @param string $resource
   *   API resource.
   *
   * @return $this
   *   APICall instance.
   */
  public function resource($resource) {
    $this->url .= "/$resource";

    return $this;
  }

  /**
   * Configure query.
   *
   * @param array $variables
   *   Query parameters.
   *
   * @return $this
   *   APICall instance.
   */
  public function query(array $variables) {
    $this->query = array_merge($this->query, $variables);

    return $this;
  }

  /**
   * Get a page of results.
   *
   * @param int $page
   *   Page number.
   *
   * @return $this
   *   APICall instance.
   */
  public function page($page = 0) {
    if ($page > 0) {
      $this->query['page'] = $page;
    }

    return $this;
  }

  /**
   * Perform a request to an API.
   *
   * @param array $options
   *   Additional options for drupal_http_request().
   *
   * @return stdClass|bool
   *   Query result or FALSE.
   */
  public function execute($options = array()) {
    if (!empty($this->query)) {
      $this->url .= '?' . http_build_query($this->query);
    }

    $this->result = drupal_http_request($this->url, array_merge(array('timeout' => 3000), $options));

    return 200 == $this->result->code ? json_decode($this->result->data) : FALSE;
  }

  /**
   * Get waw results of a query.
   *
   * @return stdClass
   *   Query result.
   */
  public function getRawResult() {
    if (empty($this->result)) {
      throw new \RuntimeException('You must call the "execute" method firstly.');
    }

    return $this->result;
  }

}
