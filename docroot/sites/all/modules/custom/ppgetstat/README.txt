There are two queues.

First one (statsJobs) is to trigger scanning jobs. Like scan user X for new
commits since Y date. This queue is filled daily.

Second queue (dorgScrapping) is for actual scrapping drupal.org pages and
scanning them. This queue is filled while processed jobs from first queue.
