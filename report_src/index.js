const express = require("express");
const readline = require("readline");
const fs = require("fs");

// read file line by line and parse the formatted data
async function processLineByLine(filepath) {
  const fileStream = fs.createReadStream(filepath);
  let result = [];

  const rl = readline.createInterface({
    input: fileStream,
    crlfDelay: Infinity,
  });

  for await (const line of rl) {
    let value = line.split(":");
    let time = line.split(" ");
    result.push({value: parseFloat(value[1]), time: time[1]});
  }
  return result;
}

// compares two files line by line and returns a result array of objects
const compareLogs = async (filepath_orig, filepath_received) => {
  let original_values = await processLineByLine(filepath_orig);
  let received_values = await processLineByLine(filepath_received);
  let result = [];

  for (let i = 0; i < original_values.length; i++) {
    result.push({
      original: original_values[i]?.value,
      origin_time: original_values[i]?.time,
      received: received_values[i]?.value,
      received_time: received_values[i]?.time,
      error: original_values[i]?.value != received_values[i]?.value ? true : false,
    });
  }
  return result;
};

// initialize express app that responds to calls on localhost:3000
const app = express();

// returns the website
app.get("/", async (req, res) => {
  res.sendFile("./index.html", { root: __dirname });
});

// gets all logfiles in log directory
app.get("/all_logs", async (req, res) => {
  let result = [];
  fs.readdir("../log", (err, files) => files.forEach(file => {
    result.push(file);
  }));
  //this isn't good style but for now it works - somehow couln't make async work
  setTimeout(() => {res.json(result);}, 1000);
});

// new route with id param that specifies the filename
app.get("/create_report/:id", async (req, res) => {
  let data = await compareLogs(
    "../sensordata_src/sensor_simulated_data.csv",
    "../log/" + req.params.id
  );
  res.json(data);
});

// old route with fixed log file - preselected
//app.get("/create_report", async (req, res) => {
//  let data = await compareLogs(
//    "../sensordata_src/sensor_simulated_data.csv",
//    "../log/8_May_23:52:07r.log"
//  );
//  res.json(data);
//});

// start the backend app
app.listen(3000, () =>
  console.log("imqtt report server listening on port 3000")
);
