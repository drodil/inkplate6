<?php
$key = htmlspecialchars($_GET["key"]);
if($key != "YOUR_SECRET_KEY") {
    http_response_code(404);
    return;
}

$ch = curl_init("https://script.google.com/macros/s/YOUR_APPS_SCRIPT/exec");
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$data = curl_exec($ch);
curl_close($ch);

header("Content-Type: application/json; charset=utf-8");
header("Pragma: no-cache");
header("Expires: 0");
header("Content-Length: " . strlen($data));
echo $data;