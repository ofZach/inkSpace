
attribute vec3 position;
attribute vec3 next;
attribute vec3 previous;
attribute vec3 lineInfo;
uniform float focusPoint;
uniform mat4 projection;
uniform mat4 modelview;
uniform float aspect;
uniform int miter;
varying float dist;
varying float topp;
varying float endPct;






void main() {
   
  vec2 aspectVec = vec2(aspect, 1.0);
  mat4 projViewModel = projection * modelview ;
  vec4 previousProjected = projViewModel * vec4(previous, 1.0);
  vec4 currentProjected = projViewModel * vec4(position, 1.0);
  vec4 nextProjected = projViewModel * vec4(next, 1.0);

  vec4 ptNonProjected = modelview * vec4(position, 1.0);
  vec2 currentScreen = currentProjected.xy / currentProjected.w * aspectVec;
    
   float scalar = 1.0;
   if (ptNonProjected.z < focusPoint) {
       scalar = ( 1.0 - abs(ptNonProjected.z - focusPoint) / 400.0);
       if (scalar < 0.5) scalar = 0.5;
   }
    
  vec2 previousScreen = previousProjected.xy / previousProjected.w * aspectVec;
  vec2 nextScreen = nextProjected.xy / nextProjected.w * aspectVec;
  float len = (lineInfo.x);
  float orientation = lineInfo.x > 0.0 ? 1.0 : -1.0;
    //len = 10.0;
  //starting point uses (next - current)
  vec2 dir = vec2(0.0);
  if (currentScreen == previousScreen) {
    dir = normalize(nextScreen - currentScreen);
  } 
  //ending point uses (current - previous)
  else if (currentScreen == nextScreen) {
    dir = normalize(currentScreen - previousScreen);
  }
  //somewhere in middle, needs a join
  else {
    //get directions from (C - B) and (B - A)
    vec2 dirA = normalize((currentScreen - previousScreen));
      vec2 dirB = normalize((nextScreen - previousScreen));
      // no miter
//    if (miter == 1) {
//      vec2 dirB = normalize((nextScreen - currentScreen));
//      //now compute the miter join normal and length
//      vec2 tangent = normalize(dirA + dirB);
//      vec2 perp = vec2(-dirA.y, dirA.x);
//      vec2 miter = vec2(-tangent.y, tangent.x);
//      dir = tangent;
//      len = thicknesses / dot(miter, perp);
//    } else {
        dir = dirA;
    // }
  }
  vec2 normal = vec2(-dir.y, dir.x);
  normal *= len/2.0;
  normal.x /= aspect;

    
    dist = lineInfo.y;
    topp = orientation;
    endPct = lineInfo.z;
    

  vec4 offset = vec4(normal, 0.0, 1.0);
    
  gl_Position = currentProjected + offset * scalar;
    //gl_Position.z = 0.0;
    
  //gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_PointSize = 1.0;
}