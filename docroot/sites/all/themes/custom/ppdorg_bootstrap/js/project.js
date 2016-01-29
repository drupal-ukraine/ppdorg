(function ($) {
  $(document).ready(function() {
   $('.node-webform select').each(function(){
       $(this).selectpicker();
   });

      $('#block-locale-language ul.language-switcher-locale-url').on('click', function(){
          $('#block-locale-language ul.language-switcher-locale-url li .glyphicon').toggleClass('arrow-up');
          $(this).children('li').toggleClass('open');
      });

  });
})(jQuery);