#!/bin/sh
drush -vy si ppdorg --db-url=mysql://drupal:drupal@127.0.0.1:/drupal --account-name=admin --account-pass=admin
