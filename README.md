# OpenGL-basics
This project was taken outside of university in order to expand my knowledge of real-time rendering techniques. I decided to implement a physically based rendering system as I found it to be a key component of modern graphics engines, enabling high quality renders in real-time. I also found the maths behind its implementation to be fascinating.

The project also gave me an opportunity to improve upon my C++ and OpenGL knowledge that I learned during my third year of university. I took the opportunity to explore and gain hands-on experience of external libraries such as ImGUI and GLM both of which are used widely within the software and graphics engineering industry.

The project implements the Cook-Torrance BRDF to calculate a value for the render equation. Within this BRDF, the values of D, F, and G are calculated using Trowbridge-Reitz GGX, Schlick-GGX, and the Fresnel-Schlick approximation.

A fragment shader calculates these values using texture inputs for albedo, roughness, ambient occlusion, and metallicity allowing for the use of complex materials within the system. These levels can be fine-tuned my a texture artist to produce a convincing effect for practically any material

Values can be updated in real-time through the implementation of the ImGUI library providing a simple demonstration of Physically based rendering. I hope to continue to add to this project by implementing new features such as real-time shadows and ambient occlusion.

<h2>Features:</h2>

<h3>ImGUI</h3>
ImGUI has been implemented, allowing for easy experimentation and adjustments.

<h3>Physically Based Rendering</h3>
The project implments a physically based rendering shader and material system using the Cook-Torrance BDRF 

<br><br>

<img src='https://raw.githubusercontent.com/wkershaw/OpenGL-basics/master/Screenshots/PBR.PNG' width='40%'>
<img src='https://raw.githubusercontent.com/wkershaw/OpenGL-basics/master/Screenshots/PBR2.PNG' width='40%'>

<h2>Known Issues:</h2>

<h2>References:</h2>
<a href='https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw'>The Cherno</a>
<br>
<a href='https://learnopengl.com/'>Learn OpenGL</a>
