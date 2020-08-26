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
// PROJECTS PAGE POPULATION
// -----------------------------------------------------------------------------

// get github projects
function displayGithubRepos() 
{
	$.getJSON( "https://api.github.com/users/gabriserra/repos", function(data) { 
		appendData(data); 
	});
}

// get a list of github repos and sort them by created_at key
function sortReposCreationDate(repos) 
{
	repos.sort(function (a, b) { 
		return compareTimestamps(a.created_at, b.created_at) 
	});
}

// iterate over github repos and populate project page
function appendData(data) 
{
	var place;
	var year;

	sortReposCreationDate(data);
	
	place = $(".archive");
	year = getYear(data[0].created_at);

	place.append("<h2 class='archive-year'>"+year+"</h2>");

	for (i = 0; i < data.length; i++)
	{
		if (year != getYear(data[i].created_at))
		{
			year = getYear(data[i].created_at);
			place.append("<h2 class='archive-year'>"+year+"</h2>");
		}

		place.append(
			"<div class='archive-item'> \
				<span class='post-date archive-date'>"+formatDate(data[i].created_at)+"</span> \
				<a href="+data[i].html_url+" class='archive-title'>"+data[i].name+"</a> \
				<p class='post-description'>"+data[i].description+"</p> \
			</div>");
	}
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


// take a timestamp and returns the year
function getYear(timestamp)
{
	return new Date(timestamp).getFullYear();
}

// format date such as Aug 13, 20
function formatDate(timestamp)
{
	var options = { year: '2-digit', month: 'short', day: 'numeric' };
	var date  = new Date(timestamp);
	
	return date.toLocaleDateString("en-US", options);
}

// take two timestamps and compare them
function compareTimestamps(tA, tB) 
{
	var dA = new Date(tA);
	var dB = new Date(tB);

	return (dA < dB ? 1 : -1);
}