import random

# Open a file for writing
with open("sensor_simulated_data.txt", "w") as file:
    for i in range(100):
        temp = round(random.uniform(-20, 35), 4)
        file.write("Temperature:{:.4f}\n". format(temp))
