/**
 * GA.JS
 * Contains Google Analytics methods
 */

// -----------------------------------------------------------------------------
// GA ANALYTICS ROUTINE
// -----------------------------------------------------------------------------

bootstrapga();

function gtag()
{
    dataLayer.push(arguments);
}

function bootstrapga()
{
    window.dataLayer = window.dataLayer || [];
    
    gtag('js', new Date());
    gtag('config', 'UA-165956539-1');
}