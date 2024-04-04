#version 400 core

in vec2 uv;
out vec4 FragColor;

uniform float u_time;
uniform vec3 origin;
uniform mat3 rotMat;

const float NUM_OF_STEPS = 1000000000.0;
const float MIN_DIST_TO_SDF = 0.0000000001;
const float MAX_DIST_TO_TRAVEL = 6.0;

float de ( vec3 p ){
  float S = 1.0f;
  float R, e;
  p.y += p.z;
  float time = u_time / 100.;
  p = vec3( log( R = length( p ) ) - time, asin( -p.z / R ), atan( p.x, p.y ) + time );
  for( e = p.y - 1.5f; S < 6e2; S += S )
    e += sqrt( abs( dot( sin( p.zxy * S ), cos( p * S ) ) ) ) / S;
  return e * R * 0.1f * 0.9;
}

float rayMarch(vec3 ro, vec3 rd, float maxDistToTravel)
{
  float dist = 0.0;

  for(float i = 0.0; i < NUM_OF_STEPS; i++)
  {
    float toMaxSteps = (dist / maxDistToTravel * 99999.) + 1.;
    float minDist = MIN_DIST_TO_SDF * toMaxSteps;
    vec3 currentPos = ro + rd * dist;
    float distToSdf = de(currentPos);

    if(distToSdf < minDist)
      break;

    dist = dist + distToSdf;

    if(dist > maxDistToTravel)
      break;
  }

  return dist;
}

vec3 getNormal(vec3 p, float pre)
{
  vec2 d = vec2(pre, 0.0);
  float gx = de(p + d.xyy) - de(p - d.xyy);
  float gy = de(p + d.yxy) - de(p - d.yxy);
  float gz = de(p + d.yyx) - de(p - d.yyx);
  vec3 normal = vec3(gx, gy, gz);
  return normalize(normal);
}

vec3 render(vec2 uv)
{
  vec3 fog = vec3(0.529, 0.808, 0.929);
  vec3 color = fog;

  // note: ro -> ray origin, rd -> ray direction
  vec3 ro = origin;
  vec3 rd = normalize(rotMat * vec3(uv, 1.0));

  float dist = rayMarch(ro, rd, MAX_DIST_TO_TRAVEL);

  if(dist < MAX_DIST_TO_TRAVEL)
  {
    // part 2.1 - calculate normals
    vec3 p = ro + rd * dist;
    float pre = (pow(dist / MAX_DIST_TO_TRAVEL, 2) * 999.) + 1.;
    vec3 normal = getNormal(p, pre / 10000.);
    color = normal;


    // part 2.2 - add lighting
    vec3 lightColor = vec3(1., 1., 0.5);
    vec3 lightSource = vec3(0., 1., 0.);
    float diffuseStrength = max(0.0, dot(normalize(lightSource), normal));
    vec3 diffuse = lightColor * diffuseStrength;
    vec3 viewSource = normalize(ro);
    vec3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.0, dot(viewSource, reflectSource));
    specularStrength = pow(specularStrength, 64.0);
    vec3 specular = specularStrength * lightColor;
    vec3 lighting = diffuse * 0.7325 + specular * 0.2325 + lightColor * 0.0125;
    color = lighting;

    // part 3 - add shadows
    vec3 lightDirection = normalize(lightSource);
    float distToLightSource = length(lightSource - p);
    ro = p + normal * 0.1;
    rd = lightDirection;
    float sdist = rayMarch(ro, rd, distToLightSource);
    if(sdist < distToLightSource)
      color = color * vec3(0.9);

	  float depth = min(dist / MAX_DIST_TO_TRAVEL, 1.);

    color = depth * fog + (1. - depth) * pow(color, vec3(1.0 / 2.2));
  }

  return color;
}

void main()
{
  vec3 color = render(uv);
  FragColor = vec4(color, 1.);
}