/**
 * MAIN.JS
 * Contains useful js methods.
 */

// -----------------------------------------------------------------------------
// DOCUMENT READY ROUTINES
// -----------------------------------------------------------------------------

// execute when page loaded
$(document).ready(
	function() { clock(); }
);

// update clock every 30 seconds
function clock() 
{
	updateClock();
	setInterval(updateClock, 30 * 1000);
}

// -----------------------------------------------------------------------------
// UTILITIES
// -----------------------------------------------------------------------------

// get current clock and insert it into tray
function updateClock()
{
	var date = new Date();
	$('.start-bar-tray-clock').html(
		date.getHours() + ":" + minutesLeadingZeros(date)
	);	
}

// get minutes from date in 'mm' format
function minutesLeadingZeros(date) 
{ 
	return (date.getMinutes() < 10 ? '0' : '') + date.getMinutes();
}
