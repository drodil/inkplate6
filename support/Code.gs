const doGet = () => {
  var calendarId = 'cercqr3un7d837v8234e4qrrnk@group.calendar.google.com';
  var now = new Date();
  var events = Calendar.Events.list(calendarId, {
    timeMin: now.toISOString(),
    singleEvents: true,
    orderBy: 'startTime',
    maxResults: 20
  });

  var json = [];

  for (var i = 0; i < events.items.length; i++) {
    var event = events.items[i];
    var start = new Date(event.start.dateTime);
    var end = new Date(event.end.dateTime);
    var fullDay = false;
    if (event.start.date) {
      // All-day event.
      start = new Date(event.start.date);
      end = new Date(event.end.date);
      fullDay = true;
    }

    json.push(
      {
        'name': event.summary, 
        'start': start.toJSON(),
        'end': end.toJSON(),
        'fullDay': fullDay
      },
    );
  }

  return ContentService.createTextOutput(JSON.stringify(json)).setMimeType(
    ContentService.MimeType.JSON
  );
};