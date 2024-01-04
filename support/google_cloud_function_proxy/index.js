const functions = require('@google-cloud/functions-framework');
const fetch = (...args) => import('node-fetch').then(({default: fetch}) => fetch(...args));

functions.http('calendar', async (req, res) => {
  const secret = req.query.secret;
  // IF YOU WANT TO PREVENT USAGE WITHOUT KEY, USE QUERY PARAMETER FOR THIS
  if(secret !== '<YOUR SECRET>') {
    res.status(404);
    res.send('Unauthorized');
    return;
  }

  // ADD YOUR GOOGLE APPS SCRIPT ID TO THE URL
  const response = await fetch('https://script.google.com/macros/s/<SCRIPT_ID>/exec');
  const data = await response.json();

  res.setHeader('Content-Type', 'application/json');
  res.send(JSON.stringify(data));
});
