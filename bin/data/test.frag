
//#version 120
//#extension GL_ARB_texture_rectangle: enable

#ifdef GL_ES
precision mediump float;
#endif


uniform sampler2D ink;
varying float dist;
varying float topp;
varying float endPct;
uniform float drawTrans;

// 500x300

//uniform sampler2DRect ink;

void main() {
    
    float y = topp*0.5 + 0.5;
    
    
    float x = mod(dist/2.0, 1000.0) / 500.0;
    
    if (x > 1.0) x = 2.0 - x;
    
    vec2 st = vec2(x, (17.0 + y * (83.0 - 17.0))/300.0);
    vec4 pix  = texture2D(ink, st);
    

    
    //pix *= 1.1;
    if (drawTrans > 0.5){
    gl_FragColor = vec4(0.1, 0.1, 0.1, 1.0); //vec4(); //pix * 0.2 + 0.8 * vec4(1.0);
    } else {
        gl_FragColor = pix; // * 0.2 + 0.8 * vec4(1.0);
    }
}