const express = require("express");
const readline = require("readline");
const fs = require("fs");

async function processLineByLine(filepath) {
  const fileStream = fs.createReadStream(filepath);
  let result = [];

  const rl = readline.createInterface({
    input: fileStream,
    crlfDelay: Infinity,
  });

  for await (const line of rl) {
    let temp = line.split(':');
    result.push(parseFloat(temp[1]));
  }
  return (result);
}

const compareLogs = async (filepath_orig, filepath_received) => {
  let original_values = await processLineByLine(filepath_orig);
  let received_values = await processLineByLine(filepath_received);
  let result = [];

  for (let i = 0; i < original_values.length; i++)
  {
    if (original_values[i] === received_values[i]){
      console.log(`diff at original: ${original_values[i]} received ${received_values[i]}`);
      result.push(`diff at original: ${original_values[i]} received ${received_values[i]}`);
    }
  }
  return (result);
}

// initialize express app that responds to calls on localhost:3000
const app = express();

app.get('/', async (req, res) => {
  let diff = await compareLogs('../sensordata_src/sensor_simulated_data.txt', '../log/5_May_14:14:37.log')
  res.send(diff);
})

app.listen(3000, () => console.log("imqtt report server listening on port 3000"));
