#ink space

ink space is a experimental drawing tool which uses the accelerometer on your android device to move the drawings you make in 3d.  It's part of the android experiments initiative. 

The project itself it fairly straight forward you can draw, move the phone by tilting in different directions, adjust line that you are drawing, create an animted line which pulses and re-draws itself and record an animated gif of whatever you make.   Double-tapping (or hitting the trash icon in the menu) clears the app.

As an artist I'm constantly thinking about new types of drawing tools, and what does drawing in the 21st century look like -- ink space is research in that realm.  If have a drawing basically in your hands, what does it look like to move around that drawing and experience more as a dimensional form that requires you to both draw and move. 

## credits
 
This project owes a great deal to Amit Pitaru's Rhonda, which I helped make native apps for, and to Matt Deslaurier's article ["drawing lines is hard"](http://mattdesl.svbtle.com/drawing-lines-is-hard). I'd also like to thank Anthony Tripaldi for some android info and Arturo Castro (as well as the broader openframeworks community).  The app is written in OF 0.8.4.

# technical details

So let's nerd out a bit!  The way this app works is that it reads data from the accelerometer and manipulates drawings by rotating them based on the phone movement. Every new point that's entered is entered in 2d (ie, it has z value of zero) but as you move the phone it gets manipulated in 3d.  Typically most graphical applications don't change the underlying points of the model but alter a matrix (a box of numbers) to manipulate the points, but here, for several reasons, it's easier and faster to actually the manipulate the points. 

So, we have a polyline (that's a set of points) that been manipulated to be in 3d - how do we go about drawing it.  Here, I have to thank the excelled Drawing Lines is Hard article (see "Screen-Space Projected Lines") for some tips about how to project lines in the shader so that they are "volumentric" -- ie, we can pass in just the polyline data (plus a little extra info) and on the shader side compute the screen coordinates of a "ribbon" like shape to give the line depth.  We can even (as I do in this app) thin or thicken the line based on it's distance away from the viewer.  This is how some cheap DOF line shaders work, fyi.  

Another important trick is that opengl on mobile devices tends to be quite slow, and there are huge penalties for how you do your drawing -- ie, if you have lots of state changes, etc.   In our case, we want to draw lines as fast as possble, so we batch them into one giant VBO (vertex buffer object).  But, we don't want them to be connected.  There's a trick where you pass in a kind of degenerate shape between lines (by duplicating the last vertices of the previous line and the first vertices of the next line).  It's explained pretty well here: 

https://developer.apple.com/library/ios/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/TechniquesforWorkingwithVertexData/TechniquesforWorkingwithVertexData.html

batching has huge implications for performance and the more you can put all your draw calls in one place the better on gles.

There's also some issues with opengl and android where the context gets lost when you pause or close the app.  openFrameworks handles most of this, but I found I had to a fair amount of destroying and reallocating for things like FBOs and VBOs.  It's a pain but once you get the hang of it (and understand it) it's bearable. 

In this app we also talk to java side using the ofActivity.java -- here, we use JNI to handle the gif sharing as well as try to make the app fully fullscreen (ie, immersive mode).  This took a lot of trial and error to get right, but once we did, it was pretty powerful to call on the android sdk and use it for what it was good at.

