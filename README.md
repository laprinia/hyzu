
## 💡🍮 hyzu

OpenGL lighting project made from scratch, destined to be a volumetric light implementation.

Includes:

* Blinn-Phong model.
* Multiple light sources: directional, point, spotlight.
* Normal mapping.
* Shadow mapping with the use of a Depth map rendered from the light's point of view projected in perspective.
* HDR, Gamma Correction and Grain as Post Processing effects.
* Anti Aliasing, Blending, Depth Testing and a Skybox implementation.

### 📷Satisfying before & after pictures:


This is how the model looked w/ no lighting & no post processing effects. The only thing I added was the skybox:


![framework screenshot](hyzu-visual/screenshots/1.png "Model Imported")

> yikes!

First thing first, I implemented the Blinn-Phong lighting in the Fragment shader using only a directional light. I also used Normal mapping to get better specular highlights. This is the result:

![framework screenshot](hyzu-visual/screenshots/2.png "Model with Blinn-Phong lighting")

The colors are still crunchy and that is because there is no Gamma Correction. I added a different shader for Post Processing with Gamma Correction & HDR. Using another framebuffer to render a scene into a texture, and then applying the Post Processing lead to this result:

![framework screenshot](hyzu-visual/screenshots/3.png "Model with Gamma Correction/HDR")

>the colors are no longer oversaturated and they look pretty ok!

Time for shadows! I needed another framebuffer, a depth one, that I rendered the whole scene in, but from my directional light's point of view (which is using a perspective projection). Again, the result of the render is put into a texture. Debugging the result I got this:

![framework screenshot](hyzu-visual/screenshots/4.png "Model Depth from light PoV")

>spooky!

Projecting the texture onto my model looks something like this:

![framework video](hyzu-visual/screenshots/4.1.gif "Model w/ Depth texture")

Now adding the computed shadow to my directional light's diffuse & specular components, I got this:

![framework video](hyzu-visual/screenshots/5.png "Model w/ Shadow Mapping")

>gorg

The lighting is pretty boring tho, so I added two point lights near the ends of the pool, and two spotlights that start from the lamps on the side, so it can fit the pool vibe more. Here they are:

![framework video](hyzu-visual/screenshots/6.png "Model w/ Multiple Lights")

One more thing I added was a Grain effect, again in my Post Processing shaders.

![framework video](hyzu-visual/screenshots/7.png "Model w/ Grain")
![framework video](hyzu-visual/screenshots/8.png "Model w/ Grain")

Now everything is set for some much needed Volumetric light & reflections/refractions.

For Volumetric lighting, I used the occlusion texture & ray marching technique.

![framework screenshot](hyzu-visual/screenshots/9.png "Light rendered with the occlusion texture")
![framework screenshot](hyzu-visual/screenshots/10.png "Combined result")

### 📷Video of the final result:

![framework video](hyzu-visual/screenshots/11.gif "Combined result video")

### New GUI with Scene Management:

![framework screenshot](hyzu-visual/screenshots/12.png "GUI")