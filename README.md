# TinyODL-TFG
Final degree thesis developing a TinyML app related with on-device training. 

Folder content explanation:
  - DualCore: Code to use the Dual-Core processor from Portenta H7 in PlatformIO IDE
  - DualCoreIDE: Code to use the Dual-Core processor from Portenta H7 in Arduino IDE
  - FL_with_CNN: Code to develop a Federated Learning application in PlatformIO IDE
  - TinyML Wake Word: Zip file with the TinyML wake word app.
  - TinyOL/hello-tiny-ml: Code with TinyML model to detect a person. The idea was to modify this code to implement the TinyOL approach. It does not work.
  - TinyOL/image-recognition: Code with mobilenet model to create an TinyOL image recognition app. It does not work
  - TinyOL/\*mobilenet\*: Weights of the mobilenet model.
  - TinyOL/convert_tflite.py: Code to convert model weights into TFLite model weights (transform to a cc array and quantized if demanded).
  - Useful code: Some codes I found useful to preserve when developing the project.
