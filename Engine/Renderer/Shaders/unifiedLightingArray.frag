#version 330 core

uniform sampler2D gTexDiffuse;
uniform sampler2D gTexNormal;
uniform sampler2D gTexSpecular;
uniform sampler2D gTexEmissive;

uniform vec3 gCamPos;

//uniform vec4 gAmbientLight;
uniform float gLightCount;
uniform float gLightPowers[16];
uniform vec4 gLightColors[16];

uniform vec2  gLightIntensities[16];
uniform vec3 gLightPositions[16];
uniform vec3 gLightFacings[16];

uniform float gLightPositionFactors[16];
uniform float gLightFacingFactors[16];
uniform float gLightAngles[16];
uniform float gSpotPowers[16];

//uniform vec4 gLightColor;
uniform float gNear;
uniform float gFar;

in vec3 passPosition;
in vec3 passLightColor;
in vec2 passUV0;
in vec3 passNormal;
in vec3 passTangent;
in vec3 passBiTangent;
in mat3 passTBN;
in vec3 passCamPos;

out vec4 outColor;


 vec3 AddDiffuseLighting(vec4 myLightCol, vec3 lightPos, vec3 worldPos, float lightPosFactor, float lightFacingFactor, vec3 lightFacing, float lightAngle, float lightPower, vec2 lightIntensity, vec3 norm){
 	//unified lighting
    float distanceLightToSurface = length(lightPos - worldPos );
    // vec3 lightFacing = normalize(gLightFacing);
    vec3 lightVector = (lightPosFactor)*( ( lightPos - worldPos  ) / distanceLightToSurface ) - ( lightFacingFactor * lightFacing);
    float spotFactor = dot(lightVector , -lightFacing);
    //gLightAngle = -2.0f;
    float intensityFactor = step(lightAngle, spotFactor);
    float lightIntensityValue = intensityFactor * (lightPower / distanceLightToSurface );// * ( pow(spotFactor,gSpotPower) * (1.0f - intensityFactor) );
    lightIntensityValue = clamp(lightIntensityValue, lightIntensity.x, lightIntensity.y );

   return ( myLightCol.xyz * lightIntensityValue * dot(norm, lightVector)  );// * diffuse.xyz;
 }

vec3 AddSpecularLighting(vec4 myLightCol, vec3 lightPos, vec3 worldPos, vec3 camPos, vec3 norm, float gloss,vec3 specular ){
	//calc specular
   vec3 towardsLightSource = lightPos - worldPos;

   vec3 eyeDir = camPos - worldPos;
   eyeDir = normalize(eyeDir);
   //eyeDir = -eyeDir;

   towardsLightSource = normalize(towardsLightSource);
   vec3 specularReflection = reflect(eyeDir ,norm); //2.0f * (dot(normal,  towardsLightSource ) * normal - towardsLightSource;
   float glossiness = gloss;
   	//vec3 mSpec = vec3(1.0f); 
   	//vec3 mSpec = diffuse.xyz;
   	vec3 mSpec = specular.xyz;
   	vec3 outSpecColor = myLightCol.xyz * mSpec * pow(dot(eyeDir, specularReflection), glossiness);
   	outSpecColor = clamp(outSpecColor,vec3(0.0f), vec3(1.0f));

   	return outSpecColor;
}

vec3 PrepareNormals(vec3 normal){

   //range map the normals
   float newNormalX = 2.0f * normal.x - 1.0f; //range map Y
   float newNormalY = 2.0f * normal.y - 1.0f; //range map Z
   float newNormalZ = normal.z; //x is neg z
   normal = vec3(newNormalX, newNormalY, newNormalZ); //local space mapping

   //normal space to world space
   mat3 passTBN = mat3(passTangent, passBiTangent, passNormal );
   //normal = normalize(normal);
   normal = passTBN * normal;
   //normal = normal * passTBN;   
   normal = normalize(normal);
  //normal = passNormal;

  return normal; 
}

vec3 GetAmbientLight( vec4 ambientLight, vec3 diffuse ){
   		vec3 ambient = (ambientLight.xyz * ambientLight.a).xyz;
   		//diffuse = vec3(1.0f);
   		return ambient * diffuse;
}

//------------------------------------------------------------------------
void main( void )
{
	int lightArraySize = int( floor(gLightCount) );

	vec4 gAmbientLight = vec4( 1.0f, 1.0f, 1.0f, .5f );

	vec4 myLightColors[16];
	for(int i = 0; i < lightArraySize; i++){
		myLightColors[i] = vec4(gLightColors[i].xyz, gLightPowers[i] );
	}

	
   // Sample the textures
   vec3 diffuse = texture( gTexDiffuse, passUV0 ).xyz;
   //get specular texture 
   vec3 specular = texture(gTexSpecular, passUV0).xyz;
    // Use texture normal - need to convert color-space to a real-space
   vec3 normal = texture( gTexNormal, passUV0 ).xyz;
   normal = PrepareNormals(normal);

   vec3 world_pos = passPosition;

   vec3 final_color = vec3(0.0f);


   // Ambient is added once
   final_color = GetAmbientLight(gAmbientLight, diffuse);
   
   //ambient once + sum of diffuse, specular

   //sum all diffuse lights
   vec3 diffuseLightingComponent = vec3(0.0f);
   for(int i = 0; i < lightArraySize; i++){
      diffuseLightingComponent += AddDiffuseLighting(myLightColors[i], gLightPositions[i], world_pos, gLightPositionFactors[i], gLightFacingFactors[i], gLightFacings[i], gLightAngles[i], gLightPowers[i], gLightIntensities[i], normal) * diffuse.xyz;
   }
   diffuseLightingComponent = clamp(diffuseLightingComponent, vec3(0.0f), vec3(1.0f));
   final_color += diffuseLightingComponent;
 //  final_color *= diffuse.xyz;


   //sum all specular lights
    vec3 specularLightingComponent = vec3(0.0f);
   for(int i = 0; i < lightArraySize; i++){
    //specComponent = clamp( specComponent , vec3(0.0f), vec3(1.0f) );
    specularLightingComponent += AddSpecularLighting(myLightColors[i], gLightPositions[i], world_pos, passCamPos, normal, 2.0f, specular  );
   }
   specularLightingComponent = clamp(specularLightingComponent, vec3(0.0f), vec3(1.0f));
   final_color += specularLightingComponent;

    vec3 emissive = texture( gTexEmissive, passUV0 ).xyz;
    final_color += emissive.xyz;
    

   /*
   //fog effect
    float fogFactor = 0.0f;//gCamPos.x / world_pos.x;
   float distanceToFog =  length(gCamPos - world_pos) ;

	fogFactor = (distanceToFog - gNear) / (gFar - gNear);
  
  // fogFactor = clamp(fogFactor, 0.0f, 1.0f);
   vec3 fogColor = vec3(0.3f, 0.3f, 0.3f);

   //fogColor = fogFactor * fogColor + (1.0f - fogFactor) * final_color;
   final_color = mix(fogColor, final_color, fogFactor); //this is a linear interpolation for fog
   
   //show fogFactor depth(gray levels); similar to depth buffer
//fogFactor = 1 - fogFactor;
// outColor = vec4( fogFactor, fogFactor, fogFactor,1.0 );
	*/

	//final_color = vec3(lightIntensity);
	//final_color = diffuse;

   // clamp it, and send it on
  final_color = clamp(final_color, 0.0f, 1.0f);

  outColor = vec4( final_color, 1.0f );

   


    //outColor = vec4(BiTanAsColor, 1.0f);
}