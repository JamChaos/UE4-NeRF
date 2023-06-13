# UE4-NeRF:Neural Radiant Field for Real-Time Rendering of Large-Scale Scene

Let's use NeRF to achieve real-time interactive rendering of large-scale scenes!

![image-20230612155747476.png](https://s2.loli.net/2023/06/12/fYGynvzS1Ml9we3.png)



## Requirement

- **Windows**
- **RTX 2000 & 3000 series**. A single 3090 GPU can support real-time rendering of an area of approximately 600m\*600m with a resolution of 1920*1080 and a frame rate of 40 frames per second.
- **cuda 11.5**

## Usage

1. Download Unreal Engine 4,  version **4.25.4** is recommended. Then, create a game project category using C++.
2. Place the plugin files in the UE4 project directory.  Two larger plugin subfiles are placed in the [google drive](https://drive.google.com/drive/folders/19PKH6zTlA1YpuhG0BNINHeWH-7CKiHzm?usp=drive_link) (Plugins/nerf_manager/Binaries and Plugins/nerf_manager/lib) and we provide  models of the "highway" scenario. 
3. Launching in Visual Studio and Configuration. Put the ".json" file path in the "Cfg path" and the "output_models" folder path in the "Model path". 

![image-20230612161410932.png](https://s2.loli.net/2023/06/12/lzT2q34eCdWyVig.png)

4. Currently, you have the flexibility to freely navigate within the scene and have the ability to dynamically add objects to the scene.



