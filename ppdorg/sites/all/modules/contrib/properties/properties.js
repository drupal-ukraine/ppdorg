(function ($) {

/**
 * Find all children of rowObject by indentation.
 *
 * Overrides the default implementation to allow non-draggable children.
 *
 * @todo: Write a core patch.
 *
 * @param addClasses
 *   Whether we want to add classes to this row to indicate child relationships.
 */
Drupal.tableDrag.prototype.row.prototype.findChildren = function (addClasses) {
  var parentIndentation = this.indents;
  var currentRow = $(this.element, this.table).next('tr.draggable, tr.properties-tabledrag-children');
  var rows = [];
  var child = 0;
  while (currentRow.length) {
    var rowIndentation = $('.indentation', currentRow).length;
    // A greater indentation indicates this is a child.
    if (rowIndentation > parentIndentation) {
      child++;
      rows.push(currentRow[0]);
      if (addClasses) {
        $('.indentation', currentRow).each(function (indentNum) {
          if (child == 1 && (indentNum == parentIndentation)) {
            $(this).addClass('tree-child-first');
          }
          if (indentNum == parentIndentation) {
            $(this).addClass('tree-child');
          }
          else if (indentNum > parentIndentation) {
            $(this).addClass('tree-child-horizontal');
          }
        });
      }
    }
    else {
      break;
    }
    currentRow = currentRow.next('tr.draggable, tr.properties-tabledrag-children');
  }
  if (addClasses && rows.length) {
    $('.indentation:nth-child(' + (parentIndentation + 1) + ')', rows[rows.length - 1]).addClass('tree-child-last');
  }
  return rows;
};

  Drupal.behaviors.propertiesUpdateTabIndex = {
    attach: function (context) {

        $(document).bind('mouseup', function (event) {
            var tabindex = 1;
            $('table.properties-table input.properties-value').each(function(context) {
                $(this).attr('tabindex', tabindex++);
            })
        });
    }
  }
})(jQuery);


