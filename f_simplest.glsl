#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform sampler2D textureMap2;
uniform sampler2D textureMap3;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 l;
in vec4 v;
in vec2 iTexCoord0;

vec2 parallax(vec2 t, float h, float s,  vec4 v) {
	vec2 ti = -h*v.xy/s;
	float hi = -v.z/s;
	vec2 tc = t;
	float hc = h;

	float ht = texture(textureMap2, tc).r * hc;

	if (v.z <= 0) {
	discard;
	} else {
		while (ht < hc) {
			tc += ti;
			if ( tc.x > 1 || tc.y > 1 || tc.x < 0 || tc.y < 0) {
				break;
			}
			hc += hi;
			ht = texture(textureMap2, tc).r * h;
		}
		if ( tc.x > 1 || tc.y > 1 || tc.x < 0 || tc.y < 0) {
				discard;
		}
	}
	return tc;
};

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml = normalize(l);
	vec4 mv = normalize(v);

	vec2 tc = parallax(iTexCoord0, 0, 600,mv);
	//vec4 mn = normalize(n);
	vec4 mn = normalize(vec4(texture(textureMap1,tc).rgb*2-1,0));
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);

	//Parametry powierzchni
	vec4 kd = texture(textureMap0, tc); 
	vec4 ks = texture(textureMap3, tc);

	//Obliczenie modelu oświetlenia
	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1),25);
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);
}
