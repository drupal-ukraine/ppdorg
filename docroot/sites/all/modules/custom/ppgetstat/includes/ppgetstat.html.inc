<?php
/**
 * @file
 * HTML parser.
 */

/**
 * Class PPGetStatHTML.
 */
class PPGetStatHTML {

  /**
   * @var bool
   */
  private $libxmlState;
  /**
   * @var DOMDocument
   */
  private $document;
  /**
   * @var DOMXPath
   */
  private $xpath;

  /**
   * PPGetStatHTML constructor.
   *
   * @param string $content
   *   HTML content.
   */
  public function __construct($content) {
    $this->document = new DOMDocument();
    // Handle errors/warnings and don't mess up output of your script.
    // @see http://stackoverflow.com/a/17559716
    $this->libxmlState = libxml_use_internal_errors(true);
    $this->document->loadHTML($content);
  }

  /**
   * Clear XML library errors.
   */
  public function __destruct() {
    libxml_clear_errors();
    libxml_use_internal_errors($this->libxmlState);
  }

  /**
   * Make XPath query to the document object.
   *
   * @return DOMXPath
   *   XPath object to perform queries.
   */
  public function xpath() {
    if (NULL === $this->xpath) {
      $this->xpath = new DOMXPath($this->document);
    }

    return $this->xpath;
  }

}
