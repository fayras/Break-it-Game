// Star Nest by Pablo RomÃ¡n Andrioli

// This content is under the MIT License.

// Original post by Kali https://www.shadertoy.com/view/XlfGRj

precision highp float;

uniform vec3 iResolution;           // viewport resolution (in pixels)
uniform float iTime;                 // shader playback time (in seconds)
uniform int iterations;
uniform int volsteps;
uniform float formuparam;
uniform vec3 from;

// #define iterations 17
//#define formuparam 0.53

// #define volsteps 20
#define stepsize 0.1

#define zoom   0.800
#define tile   0.850
#define speed  0.0015

#define brightness 0.002
#define darkmatter 0.800
#define distfading 0.700
#define saturation 0.750

float SCurve (float value) {

    if (value < 0.5)
    {
        return value * value * value * value * value * 16.0;
    }

    value -= 1.0;

    return value * value * value * value * value * 16.0 + 1.0;
}

void main()
{
	//get coords and direction
	vec2 uv=gl_FragCoord.xy/iResolution.xy-.5;
	uv.y*=iResolution.y/iResolution.x;
	vec3 dir=vec3(uv*zoom,1.);
	float time=iTime*speed+.25;

	// vec3 from=vec3(1.,.5,0.5);
	from+=vec3(0,time*2.,0);

	//volumetric rendering
	float s=0.1,fade=1.;
	vec3 v=vec3(0.);
	for (int r=0; r<volsteps; r++) {
		vec3 p=from+s*dir*.5;
		p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
		float pa,a=pa=0.;
		for (int i=0; i<iterations; i++) {
			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);
		}
		float dm=max(0.,darkmatter-a*a*.001); //dark matter
		a = pow(abs(a), 2.5); // add contrast
		if (r>6) fade*=1.-dm; // dark matter, don't render near
		//v+=vec3(dm,dm*.5,0.);
		v+=fade;
		v+=vec3(s*s*s*s,s*s*s*s,s*s)*a*brightness*fade; // coloring based on distance
		fade*=distfading; // distance fading
		s+=stepsize;
	}

	v=mix(vec3(length(v)),v,saturation); //color adjust

    vec4 C = vec4(v*.01,1.);

     	C.r = pow(C.r, 0.35);
 	 	C.g = pow(C.g, 0.36);
 	 	C.b = pow(C.b, 0.4);

    vec4 L = C;

    	C.r = mix(L.r, SCurve(C.r), 1.0);
    	C.g = mix(L.g, SCurve(C.g), 0.9);
    	C.b = mix(L.b, SCurve(C.b), 0.6);

	gl_FragColor = C;
}