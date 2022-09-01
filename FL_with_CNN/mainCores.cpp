// #include "Arduino.h"
// #include "RPC.h"
// #include "ModelNN.h"

// using namespace rtos;


// const int InputSize = 4;
// const int OutputSize = 1; 

// ModelNN model(1);

// Thread PortentaThread;

// int rInt(int range=20) {
//         int rand = random(range);
//         return rand - range/2;
//     }
// float rFloat(int range=100) {
//     float rand = float(random(range))/float(range);
//     return 2.0 * (rand - 0.5);
// }

// /**
//  * Returns the CPU that's currently running the sketch (M7 or M4)
//  * Note that the sketch has to be uploaded to both cores. 
//  **/
// String currentCPU() {
//   if (HAL_GetCurrentCPUID() == CM7_CPUID) {
//     return "M7";
//   } else {
//     return "M4";
//   }
// }
// void writeRPCprints() {
//     String buffer = "";
//     while (RPC.available()) {
//         buffer += (char)RPC.read(); // Fill the buffer with characters
//     }

//     if (buffer.length() > 0) {
//         Serial.print(buffer);
//     }
// }

// void setup_model() {
//     Dense outputLayer = Dense(InputSize, OutputSize);
//     model.add(&outputLayer);
// }

// int train_modelM7() {
//     while(!model.nLayers);
//     int numItems = 100;
//     float X[numItems*InputSize], Y[numItems*OutputSize];
//     for(int i=0; i < numItems; ++i) {
//         X[i*InputSize] = rFloat();
//         X[i*InputSize+1] = rFloat();
//         X[i*InputSize+2] = rFloat();
//         X[i*InputSize+3] = rFloat();
        
//         Y[i*OutputSize] = X[i*InputSize+1] * X[i*InputSize]*X[i*InputSize] +
//                           X[i*InputSize+2] * X[i*InputSize] +
//                           X[i*InputSize+3];
//     }
//     Serial.println("Starting training in " + String(currentCPU()));

//     model.train(numItems, X, Y, 100, 10);
//     return 1;
// }

// void setup_m4() {
//     int result = RPC.call("train_modelM7").as<int>();
//     RPC.println("Result: " + String(result));
// }

// void loop_m4() {
//     RPC.println("Looping M4"); delay(1000);
//     // float X[InputSize];
//     // for(int i=0; i< InputSize; ++i) X[i] = rFloat();
//     // float* result = model.predict(X);
//     // float target = X[1]*X[0]*X[0] + X[2]*X[0] + X[3];
//     // RPC.print(String(X[1]) + "*" + String(X[0]) + "*" + String(X[0]) + " + ");
//     // RPC.print(String(X[1]) + "*" + String(X[0]) + " + ");
//     // RPC.println(String(X[3]) + " :");
//     // RPC.println("\t Output: " + String(*(result)));
//     // RPC.println("\t Target: " + String(target));
// }

// void setup_m7() {
//     setup_model();
//     RPC.bind("train_modelM7", train_modelM7);
// }

// void loop_m7() {
//     String buffer = "";
//     while (RPC.available())
//         buffer += (char)RPC.read(); // Fill the buffer with characters
//     if (buffer.length() > 0) 
//         Serial.print(buffer);
// }


// void setup() {
//     RPC.begin();
//     Serial.begin(115200);

//     delay(10000);
    
//     if (currentCPU() == "M7")
//         setup_m7();
//     if (currentCPU() == "M4") 
//         setup_m4();

//     // Serial.println("Setting up model");
//     // setup_model();
//     // Serial.println("Train model");
//     // train_model();


//     Serial.println("Setup done!");
//     Serial.println("Start loop...");
// }

// void loop() {
//   if (currentCPU() == "M4") 
//       loop_m4();
//   if (currentCPU() == "M7") 
//       loop_m7();
// }
