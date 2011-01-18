/* crater4.c
 * generated wtih obj2.c from crater4.obj
 */

#include "crater4.h"

void drawcrater4(void)
{
GLfloat crater4Vertices[][3] = {
  {0.156413, -0.002416, 0.007709},
  {0.000145, -0.002416, 0.000034},
  {0.635323, -0.265694, 0.000061},
  {0.503733, -0.211187, 0.039411},
  {0.143799, -0.062098, 0.007709},
  {0.192195, -0.082144, 0.023462},
  {0.249983, -0.106081, 0.057688},
  {0.327496, -0.138188, 0.179949},
  {0.400920, -0.168601, 0.093293},
  {0.645874, -0.270064, 0.000174},
  {0.493578, -0.496681, 0.000174},
  {0.306098, -0.309201, 0.093293},
  {0.249902, -0.253005, 0.179949},
  {0.190576, -0.193679, 0.057688},
  {0.146347, -0.149450, 0.023462},
  {0.109307, -0.112410, 0.007709},
  {-0.001192, -0.001912, 0.000034},
  {0.384788, -0.387891, 0.039411},
  {0.485502, -0.488606, 0.000061},
  {0.261781, -0.637216, 0.000061},
  {0.207275, -0.505626, 0.039411},
  {0.058186, -0.145692, 0.007709},
  {0.078232, -0.194088, 0.023462},
  {0.102168, -0.251876, 0.057688},
  {0.134276, -0.329389, 0.179949},
  {0.164688, -0.402813, 0.093293},
  {0.266152, -0.647767, 0.000174},
  {-0.001780, -0.700320, 0.000174},
  {-0.001780, -0.435183, 0.093293},
  {-0.001780, -0.355710, 0.179949},
  {-0.001780, -0.271811, 0.057688},
  {-0.001780, -0.209261, 0.023462},
  {-0.001780, -0.156878, 0.007709},
  {-0.001780, -0.000610, 0.000034},
  {-0.001780, -0.546468, 0.039411},
  {-0.001780, -0.688899, 0.000061},
  {-0.265058, -0.635788, 0.000061},
  {-0.210552, -0.504198, 0.039411},
  {-0.061462, -0.144265, 0.007709},
  {-0.081509, -0.192660, 0.023462},
  {-0.105445, -0.250448, 0.057688},
  {-0.137552, -0.327962, 0.179949},
  {-0.167965, -0.401385, 0.093293},
  {-0.269428, -0.646339, 0.000174},
  {-0.496046, -0.494043, 0.000174},
  {-0.308566, -0.306563, 0.093293},
  {-0.252370, -0.250367, 0.179949},
  {-0.193044, -0.191041, 0.057688},
  {-0.148815, -0.146812, 0.023462},
  {-0.111774, -0.109772, 0.007709},
  {-0.001276, 0.000726, 0.000034},
  {-0.387256, -0.385253, 0.039411},
  {-0.487970, -0.485968, 0.000061},
  {-0.636580, -0.262247, 0.000061},
  {-0.504991, -0.207740, 0.039411},
  {-0.000684, 0.001150, 0.000034},
  {-0.145057, -0.058651, 0.007709},
  {-0.193453, -0.078697, 0.023462},
  {-0.251240, -0.102634, 0.057688},
  {-0.328754, -0.134741, 0.179949},
  {-0.402177, -0.165154, 0.093293},
  {-0.647131, -0.266617, 0.000174},
  {-0.699684, 0.001315, 0.000174},
  {-0.434548, 0.001315, 0.093293},
  {-0.355075, 0.001315, 0.179949},
  {-0.271175, 0.001315, 0.057688},
  {-0.208626, 0.001315, 0.023462},
  {-0.156243, 0.001315, 0.007709},
  {-0.545832, 0.001315, 0.039411},
  {-0.688264, 0.001315, 0.000061},
  {-0.635153, 0.264593, 0.000061},
  {-0.503563, 0.210087, 0.039411},
  {0.000744, 0.001196, 0.000034},
  {-0.143629, 0.060997, 0.007709},
  {-0.192025, 0.081043, 0.023462},
  {-0.249813, 0.104980, 0.057688},
  {-0.327326, 0.137087, 0.179949},
  {-0.400750, 0.167500, 0.093293},
  {-0.645704, 0.268963, 0.000174},
  {-0.493408, 0.495580, 0.000174},
  {-0.305928, 0.308101, 0.093293},
  {-0.249732, 0.251905, 0.179949},
  {-0.190406, 0.192579, 0.057688},
  {-0.146177, 0.148350, 0.023462},
  {-0.109136, 0.111309, 0.007709},
  {-0.384618, 0.386790, 0.039411},
  {-0.485332, 0.487505, 0.000061},
  {-0.261611, 0.636115, 0.000061},
  {-0.207105, 0.504525, 0.039411},
  {0.001786, 0.000219, 0.000034},
  {-0.058016, 0.144592, 0.007709},
  {-0.078062, 0.192987, 0.023462},
  {-0.101998, 0.250775, 0.057688},
  {-0.134105, 0.328288, 0.179949},
  {-0.164518, 0.401712, 0.093293},
  {-0.265982, 0.646666, 0.000174},
  {0.001950, 0.699219, 0.000174},
  {0.001950, 0.434083, 0.093293},
  {0.001950, 0.354610, 0.179949},
  {0.001950, 0.270710, 0.057688},
  {0.001950, 0.208160, 0.023462},
  {0.001950, 0.155778, 0.007709},
  {0.001950, 0.545367, 0.039411},
  {0.001950, 0.687799, 0.000061},
  {0.265228, 0.634687, 0.000061},
  {0.210722, 0.503098, 0.039411},
  {0.001831, -0.001209, 0.000034},
  {0.061633, 0.143164, 0.007709},
  {0.081679, 0.191559, 0.023462},
  {0.105615, 0.249347, 0.057688},
  {0.137722, 0.326861, 0.179949},
  {0.168135, 0.400284, 0.093293},
  {0.269599, 0.645238, 0.000174},
  {0.496216, 0.492942, 0.000174},
  {0.308736, 0.305462, 0.093293},
  {0.252540, 0.249267, 0.179949},
  {0.193214, 0.189940, 0.057688},
  {0.148985, 0.145711, 0.023462},
  {0.111945, 0.108671, 0.007709},
  {0.001446, -0.001827, 0.000034},
  {0.387426, 0.384152, 0.039411},
  {0.488140, 0.484867, 0.000061},
  {0.636750, 0.261146, 0.000061},
  {0.505161, 0.206640, 0.039411},
  {0.145227, 0.057550, 0.007709},
  {0.193623, 0.077596, 0.023462},
  {0.251410, 0.101533, 0.057688},
  {0.328924, 0.133640, 0.179949},
  {0.402347, 0.164053, 0.093293},
  {0.647301, 0.265516, 0.000174},
  {0.699854, -0.002416, 0.000174},
  {0.434718, -0.002416, 0.093293},
  {0.355245, -0.002416, 0.179949},
  {0.271345, -0.002416, 0.057688},
  {0.208796, -0.002416, 0.023462},
  {0.546002, -0.002416, 0.039411},
  {0.688434, -0.002416, 0.000061}};

GLfloat crater4Normals[][3] = {
  {0.150212, -0.000427, 0.988647},
  {0.361187, -0.001251, 0.932463},
  {0.333201, -0.139409, 0.932463},
  {0.138615, -0.057894, 0.988647},
  {-0.009156, 0.003815, 0.999939},
  {-0.009919, 0.000000, 0.999939},
  {-0.172887, 0.073977, 0.982147},
  {-0.187445, 0.002167, 0.982238},
  {-0.046815, -0.007599, 0.998871},
  {-0.030946, 0.033265, 0.998962},
  {0.611164, -0.002686, 0.791467},
  {0.563616, -0.236366, 0.791467},
  {-0.364544, 0.154668, 0.918210},
  {-0.396008, 0.003388, 0.918210},
  {-0.078677, 0.001190, 0.996887},
  {-0.072237, 0.031190, 0.996887},
  {-0.632130, 0.266701, 0.727500},
  {-0.686056, 0.004486, 0.727500},
  {-0.481948, 0.488296, 0.727500},
  {-0.054781, 0.056490, 0.996887},
  {-0.277596, 0.282418, 0.918210},
  {0.430250, -0.434065, 0.791467},
  {-0.131413, 0.134159, 0.982177},
  {0.254494, -0.256294, 0.932463},
  {0.105899, -0.106540, 0.988647},
  {-0.006989, 0.007019, 0.999939},
  {0.137028, -0.334178, 0.932463},
  {0.057070, -0.138951, 0.988647},
  {-0.003754, 0.009186, 0.999939},
  {-0.069735, 0.174108, 0.982238},
  {0.231391, -0.565691, 0.791467},
  {-0.148381, 0.367138, 0.918210},
  {-0.028993, 0.073153, 0.996887},
  {-0.258400, 0.635578, 0.727500},
  {0.004486, 0.686056, 0.727500},
  {0.001190, 0.078677, 0.996887},
  {0.003388, 0.396008, 0.918210},
  {-0.002686, -0.611164, 0.791467},
  {0.002167, 0.188116, 0.982116},
  {-0.001251, -0.361187, 0.932463},
  {0.010865, 0.046388, 0.998840},
  {-0.000427, -0.150212, 0.988647},
  {0.000000, 0.009919, 0.999939},
  {-0.139409, -0.333201, 0.932463},
  {-0.057894, -0.138615, 0.988647},
  {0.003815, 0.009156, 0.999939},
  {0.073794, 0.172430, 0.982238},
  {-0.236366, -0.563616, 0.791467},
  {0.154668, 0.364544, 0.918210},
  {0.031190, 0.072237, 0.996887},
  {0.266701, 0.632130, 0.727500},
  {0.488296, 0.481948, 0.727500},
  {0.056490, 0.054781, 0.996887},
  {0.282418, 0.277596, 0.918210},
  {-0.434065, -0.430250, 0.791467},
  {0.134526, 0.131260, 0.982147},
  {-0.256294, -0.254494, 0.932463},
  {0.035768, 0.033753, 0.998779},
  {-0.106540, -0.105899, 0.988647},
  {0.007019, 0.006989, 0.999939},
  {-0.334178, -0.137028, 0.932463},
  {-0.138951, -0.057070, 0.988647},
  {0.009186, 0.003754, 0.999939},
  {0.046876, 0.007904, 0.998840},
  {0.174444, 0.070009, 0.982147},
  {-0.565691, -0.231391, 0.791467},
  {0.367138, 0.148381, 0.918210},
  {0.073153, 0.028993, 0.996887},
  {0.635578, 0.258400, 0.727500},
  {0.686056, -0.004486, 0.727500},
  {0.078677, -0.001190, 0.996887},
  {0.396008, -0.003388, 0.918210},
  {-0.611164, 0.002686, 0.791467},
  {0.187536, -0.002167, 0.982238},
  {-0.361187, 0.001251, 0.932463},
  {-0.150212, 0.000427, 0.988647},
  {0.009919, 0.000000, 0.999939},
  {-0.333201, 0.139409, 0.932463},
  {-0.138615, 0.057894, 0.988647},
  {0.009156, -0.003815, 0.999939},
  {0.038667, -0.027802, 0.998840},
  {0.172948, -0.074007, 0.982116},
  {-0.563616, 0.236366, 0.791467},
  {0.364544, -0.154668, 0.918210},
  {0.072237, -0.031190, 0.996887},
  {0.632130, -0.266701, 0.727500},
  {0.481948, -0.488296, 0.727500},
  {0.054781, -0.056490, 0.996887},
  {0.277596, -0.282418, 0.918210},
  {-0.430250, 0.434065, 0.791467},
  {0.131046, -0.134159, 0.982238},
  {-0.254494, 0.256294, 0.932463},
  {-0.105899, 0.106540, 0.988647},
  {0.006989, -0.007019, 0.999939},
  {-0.137028, 0.334178, 0.932463},
  {-0.057070, 0.138951, 0.988647},
  {0.003754, -0.009186, 0.999939},
  {0.017182, -0.045961, 0.998779},
  {0.069582, -0.174413, 0.982177},
  {-0.231391, 0.565691, 0.791467},
  {0.148381, -0.367138, 0.918210},
  {0.028993, -0.073153, 0.996887},
  {0.258400, -0.635578, 0.727500},
  {-0.004486, -0.686056, 0.727500},
  {-0.001190, -0.078677, 0.996887},
  {-0.003388, -0.396008, 0.918210},
  {0.002686, 0.611164, 0.791467},
  {-0.002197, -0.188055, 0.982147},
  {0.001251, 0.361187, 0.932463},
  {-0.010620, -0.046297, 0.998840},
  {0.000427, 0.150212, 0.988647},
  {0.000000, -0.009919, 0.999939},
  {0.139409, 0.333201, 0.932463},
  {0.057894, 0.138615, 0.988647},
  {-0.003815, -0.009156, 0.999939},
  {-0.073702, -0.172552, 0.982208},
  {0.236366, 0.563616, 0.791467},
  {-0.154668, -0.364544, 0.918210},
  {-0.031190, -0.072237, 0.996887},
  {-0.266701, -0.632130, 0.727500},
  {-0.488296, -0.481948, 0.727500},
  {-0.056490, -0.054781, 0.996887},
  {-0.282418, -0.277596, 0.918210},
  {0.434065, 0.430250, 0.791467},
  {-0.134312, -0.131016, 0.982208},
  {0.256294, 0.254494, 0.932463},
  {-0.035615, -0.033570, 0.998779},
  {0.106540, 0.105899, 0.988647},
  {-0.007019, -0.006989, 0.999939},
  {0.334178, 0.137028, 0.932463},
  {0.138951, 0.057070, 0.988647},
  {-0.009186, -0.003754, 0.999939},
  {-0.174566, -0.069918, 0.982147},
  {0.565691, 0.231391, 0.791467},
  {-0.367138, -0.148381, 0.918210},
  {-0.073153, -0.028993, 0.996887},
  {-0.635578, -0.258400, 0.727500}};

  glBegin(GL_QUADS);
  glNormal3fv(crater4Normals[0]); glVertex3fv(crater4Vertices[136]);
  glNormal3fv(crater4Normals[1]); glVertex3fv(crater4Vertices[135]);
  glNormal3fv(crater4Normals[2]); glVertex3fv(crater4Vertices[3]);
  glNormal3fv(crater4Normals[3]); glVertex3fv(crater4Vertices[2]);
  glNormal3fv(crater4Normals[0]); glVertex3fv(crater4Vertices[136]);
  glNormal3fv(crater4Normals[3]); glVertex3fv(crater4Vertices[2]);
  glNormal3fv(crater4Normals[4]); glVertex3fv(crater4Vertices[9]);
  glNormal3fv(crater4Normals[5]); glVertex3fv(crater4Vertices[130]);
  glNormal3fv(crater4Normals[6]); glVertex3fv(crater4Vertices[4]);
  glNormal3fv(crater4Normals[7]); glVertex3fv(crater4Vertices[0]);
  glNormal3fv(crater4Normals[8]); glVertex3fv(crater4Vertices[1]);
  glNormal3fv(crater4Normals[9]); glVertex3fv(crater4Vertices[16]);
  glNormal3fv(crater4Normals[1]); glVertex3fv(crater4Vertices[135]);
  glNormal3fv(crater4Normals[10]); glVertex3fv(crater4Vertices[131]);
  glNormal3fv(crater4Normals[11]); glVertex3fv(crater4Vertices[8]);
  glNormal3fv(crater4Normals[2]); glVertex3fv(crater4Vertices[3]);
  glNormal3fv(crater4Normals[7]); glVertex3fv(crater4Vertices[0]);
  glNormal3fv(crater4Normals[6]); glVertex3fv(crater4Vertices[4]);
  glNormal3fv(crater4Normals[12]); glVertex3fv(crater4Vertices[5]);
  glNormal3fv(crater4Normals[13]); glVertex3fv(crater4Vertices[134]);
  glNormal3fv(crater4Normals[14]); glVertex3fv(crater4Vertices[132]);
  glNormal3fv(crater4Normals[15]); glVertex3fv(crater4Vertices[7]);
  glNormal3fv(crater4Normals[11]); glVertex3fv(crater4Vertices[8]);
  glNormal3fv(crater4Normals[10]); glVertex3fv(crater4Vertices[131]);
  glNormal3fv(crater4Normals[13]); glVertex3fv(crater4Vertices[134]);
  glNormal3fv(crater4Normals[12]); glVertex3fv(crater4Vertices[5]);
  glNormal3fv(crater4Normals[16]); glVertex3fv(crater4Vertices[6]);
  glNormal3fv(crater4Normals[17]); glVertex3fv(crater4Vertices[133]);
  glNormal3fv(crater4Normals[17]); glVertex3fv(crater4Vertices[133]);
  glNormal3fv(crater4Normals[16]); glVertex3fv(crater4Vertices[6]);
  glNormal3fv(crater4Normals[15]); glVertex3fv(crater4Vertices[7]);
  glNormal3fv(crater4Normals[14]); glVertex3fv(crater4Vertices[132]);
  glNormal3fv(crater4Normals[16]); glVertex3fv(crater4Vertices[6]);
  glNormal3fv(crater4Normals[18]); glVertex3fv(crater4Vertices[13]);
  glNormal3fv(crater4Normals[19]); glVertex3fv(crater4Vertices[12]);
  glNormal3fv(crater4Normals[15]); glVertex3fv(crater4Vertices[7]);
  glNormal3fv(crater4Normals[12]); glVertex3fv(crater4Vertices[5]);
  glNormal3fv(crater4Normals[20]); glVertex3fv(crater4Vertices[14]);
  glNormal3fv(crater4Normals[18]); glVertex3fv(crater4Vertices[13]);
  glNormal3fv(crater4Normals[16]); glVertex3fv(crater4Vertices[6]);
  glNormal3fv(crater4Normals[15]); glVertex3fv(crater4Vertices[7]);
  glNormal3fv(crater4Normals[19]); glVertex3fv(crater4Vertices[12]);
  glNormal3fv(crater4Normals[21]); glVertex3fv(crater4Vertices[11]);
  glNormal3fv(crater4Normals[11]); glVertex3fv(crater4Vertices[8]);
  glNormal3fv(crater4Normals[6]); glVertex3fv(crater4Vertices[4]);
  glNormal3fv(crater4Normals[22]); glVertex3fv(crater4Vertices[15]);
  glNormal3fv(crater4Normals[20]); glVertex3fv(crater4Vertices[14]);
  glNormal3fv(crater4Normals[12]); glVertex3fv(crater4Vertices[5]);
  glNormal3fv(crater4Normals[2]); glVertex3fv(crater4Vertices[3]);
  glNormal3fv(crater4Normals[11]); glVertex3fv(crater4Vertices[8]);
  glNormal3fv(crater4Normals[21]); glVertex3fv(crater4Vertices[11]);
  glNormal3fv(crater4Normals[23]); glVertex3fv(crater4Vertices[17]);
  glNormal3fv(crater4Normals[3]); glVertex3fv(crater4Vertices[2]);
  glNormal3fv(crater4Normals[24]); glVertex3fv(crater4Vertices[18]);
  glNormal3fv(crater4Normals[25]); glVertex3fv(crater4Vertices[10]);
  glNormal3fv(crater4Normals[4]); glVertex3fv(crater4Vertices[9]);
  glNormal3fv(crater4Normals[3]); glVertex3fv(crater4Vertices[2]);
  glNormal3fv(crater4Normals[2]); glVertex3fv(crater4Vertices[3]);
  glNormal3fv(crater4Normals[23]); glVertex3fv(crater4Vertices[17]);
  glNormal3fv(crater4Normals[24]); glVertex3fv(crater4Vertices[18]);
  glNormal3fv(crater4Normals[24]); glVertex3fv(crater4Vertices[18]);
  glNormal3fv(crater4Normals[23]); glVertex3fv(crater4Vertices[17]);
  glNormal3fv(crater4Normals[26]); glVertex3fv(crater4Vertices[20]);
  glNormal3fv(crater4Normals[27]); glVertex3fv(crater4Vertices[19]);
  glNormal3fv(crater4Normals[24]); glVertex3fv(crater4Vertices[18]);
  glNormal3fv(crater4Normals[27]); glVertex3fv(crater4Vertices[19]);
  glNormal3fv(crater4Normals[28]); glVertex3fv(crater4Vertices[26]);
  glNormal3fv(crater4Normals[25]); glVertex3fv(crater4Vertices[10]);
  glNormal3fv(crater4Normals[23]); glVertex3fv(crater4Vertices[17]);
  glNormal3fv(crater4Normals[21]); glVertex3fv(crater4Vertices[11]);
  glNormal3fv(crater4Normals[30]); glVertex3fv(crater4Vertices[25]);
  glNormal3fv(crater4Normals[26]); glVertex3fv(crater4Vertices[20]);
  glNormal3fv(crater4Normals[22]); glVertex3fv(crater4Vertices[15]);
  glNormal3fv(crater4Normals[29]); glVertex3fv(crater4Vertices[21]);
  glNormal3fv(crater4Normals[31]); glVertex3fv(crater4Vertices[22]);
  glNormal3fv(crater4Normals[20]); glVertex3fv(crater4Vertices[14]);
  glNormal3fv(crater4Normals[19]); glVertex3fv(crater4Vertices[12]);
  glNormal3fv(crater4Normals[32]); glVertex3fv(crater4Vertices[24]);
  glNormal3fv(crater4Normals[30]); glVertex3fv(crater4Vertices[25]);
  glNormal3fv(crater4Normals[21]); glVertex3fv(crater4Vertices[11]);
  glNormal3fv(crater4Normals[20]); glVertex3fv(crater4Vertices[14]);
  glNormal3fv(crater4Normals[31]); glVertex3fv(crater4Vertices[22]);
  glNormal3fv(crater4Normals[33]); glVertex3fv(crater4Vertices[23]);
  glNormal3fv(crater4Normals[18]); glVertex3fv(crater4Vertices[13]);
  glNormal3fv(crater4Normals[18]); glVertex3fv(crater4Vertices[13]);
  glNormal3fv(crater4Normals[33]); glVertex3fv(crater4Vertices[23]);
  glNormal3fv(crater4Normals[32]); glVertex3fv(crater4Vertices[24]);
  glNormal3fv(crater4Normals[19]); glVertex3fv(crater4Vertices[12]);
  glNormal3fv(crater4Normals[33]); glVertex3fv(crater4Vertices[23]);
  glNormal3fv(crater4Normals[34]); glVertex3fv(crater4Vertices[30]);
  glNormal3fv(crater4Normals[35]); glVertex3fv(crater4Vertices[29]);
  glNormal3fv(crater4Normals[32]); glVertex3fv(crater4Vertices[24]);
  glNormal3fv(crater4Normals[31]); glVertex3fv(crater4Vertices[22]);
  glNormal3fv(crater4Normals[36]); glVertex3fv(crater4Vertices[31]);
  glNormal3fv(crater4Normals[34]); glVertex3fv(crater4Vertices[30]);
  glNormal3fv(crater4Normals[33]); glVertex3fv(crater4Vertices[23]);
  glNormal3fv(crater4Normals[32]); glVertex3fv(crater4Vertices[24]);
  glNormal3fv(crater4Normals[35]); glVertex3fv(crater4Vertices[29]);
  glNormal3fv(crater4Normals[37]); glVertex3fv(crater4Vertices[28]);
  glNormal3fv(crater4Normals[30]); glVertex3fv(crater4Vertices[25]);
  glNormal3fv(crater4Normals[29]); glVertex3fv(crater4Vertices[21]);
  glNormal3fv(crater4Normals[38]); glVertex3fv(crater4Vertices[32]);
  glNormal3fv(crater4Normals[36]); glVertex3fv(crater4Vertices[31]);
  glNormal3fv(crater4Normals[31]); glVertex3fv(crater4Vertices[22]);
  glNormal3fv(crater4Normals[26]); glVertex3fv(crater4Vertices[20]);
  glNormal3fv(crater4Normals[30]); glVertex3fv(crater4Vertices[25]);
  glNormal3fv(crater4Normals[37]); glVertex3fv(crater4Vertices[28]);
  glNormal3fv(crater4Normals[39]); glVertex3fv(crater4Vertices[34]);
  glNormal3fv(crater4Normals[9]); glVertex3fv(crater4Vertices[16]);
  glNormal3fv(crater4Normals[40]); glVertex3fv(crater4Vertices[33]);
  glNormal3fv(crater4Normals[38]); glVertex3fv(crater4Vertices[32]);
  glNormal3fv(crater4Normals[29]); glVertex3fv(crater4Vertices[21]);
  glNormal3fv(crater4Normals[27]); glVertex3fv(crater4Vertices[19]);
  glNormal3fv(crater4Normals[41]); glVertex3fv(crater4Vertices[35]);
  glNormal3fv(crater4Normals[42]); glVertex3fv(crater4Vertices[27]);
  glNormal3fv(crater4Normals[28]); glVertex3fv(crater4Vertices[26]);
  glNormal3fv(crater4Normals[27]); glVertex3fv(crater4Vertices[19]);
  glNormal3fv(crater4Normals[26]); glVertex3fv(crater4Vertices[20]);
  glNormal3fv(crater4Normals[39]); glVertex3fv(crater4Vertices[34]);
  glNormal3fv(crater4Normals[41]); glVertex3fv(crater4Vertices[35]);
  glNormal3fv(crater4Normals[41]); glVertex3fv(crater4Vertices[35]);
  glNormal3fv(crater4Normals[39]); glVertex3fv(crater4Vertices[34]);
  glNormal3fv(crater4Normals[43]); glVertex3fv(crater4Vertices[37]);
  glNormal3fv(crater4Normals[44]); glVertex3fv(crater4Vertices[36]);
  glNormal3fv(crater4Normals[41]); glVertex3fv(crater4Vertices[35]);
  glNormal3fv(crater4Normals[44]); glVertex3fv(crater4Vertices[36]);
  glNormal3fv(crater4Normals[45]); glVertex3fv(crater4Vertices[43]);
  glNormal3fv(crater4Normals[42]); glVertex3fv(crater4Vertices[27]);
  glNormal3fv(crater4Normals[39]); glVertex3fv(crater4Vertices[34]);
  glNormal3fv(crater4Normals[37]); glVertex3fv(crater4Vertices[28]);
  glNormal3fv(crater4Normals[47]); glVertex3fv(crater4Vertices[42]);
  glNormal3fv(crater4Normals[43]); glVertex3fv(crater4Vertices[37]);
  glNormal3fv(crater4Normals[38]); glVertex3fv(crater4Vertices[32]);
  glNormal3fv(crater4Normals[46]); glVertex3fv(crater4Vertices[38]);
  glNormal3fv(crater4Normals[48]); glVertex3fv(crater4Vertices[39]);
  glNormal3fv(crater4Normals[36]); glVertex3fv(crater4Vertices[31]);
  glNormal3fv(crater4Normals[35]); glVertex3fv(crater4Vertices[29]);
  glNormal3fv(crater4Normals[49]); glVertex3fv(crater4Vertices[41]);
  glNormal3fv(crater4Normals[47]); glVertex3fv(crater4Vertices[42]);
  glNormal3fv(crater4Normals[37]); glVertex3fv(crater4Vertices[28]);
  glNormal3fv(crater4Normals[36]); glVertex3fv(crater4Vertices[31]);
  glNormal3fv(crater4Normals[48]); glVertex3fv(crater4Vertices[39]);
  glNormal3fv(crater4Normals[50]); glVertex3fv(crater4Vertices[40]);
  glNormal3fv(crater4Normals[34]); glVertex3fv(crater4Vertices[30]);
  glNormal3fv(crater4Normals[34]); glVertex3fv(crater4Vertices[30]);
  glNormal3fv(crater4Normals[50]); glVertex3fv(crater4Vertices[40]);
  glNormal3fv(crater4Normals[49]); glVertex3fv(crater4Vertices[41]);
  glNormal3fv(crater4Normals[35]); glVertex3fv(crater4Vertices[29]);
  glNormal3fv(crater4Normals[50]); glVertex3fv(crater4Vertices[40]);
  glNormal3fv(crater4Normals[51]); glVertex3fv(crater4Vertices[47]);
  glNormal3fv(crater4Normals[52]); glVertex3fv(crater4Vertices[46]);
  glNormal3fv(crater4Normals[49]); glVertex3fv(crater4Vertices[41]);
  glNormal3fv(crater4Normals[48]); glVertex3fv(crater4Vertices[39]);
  glNormal3fv(crater4Normals[53]); glVertex3fv(crater4Vertices[48]);
  glNormal3fv(crater4Normals[51]); glVertex3fv(crater4Vertices[47]);
  glNormal3fv(crater4Normals[50]); glVertex3fv(crater4Vertices[40]);
  glNormal3fv(crater4Normals[49]); glVertex3fv(crater4Vertices[41]);
  glNormal3fv(crater4Normals[52]); glVertex3fv(crater4Vertices[46]);
  glNormal3fv(crater4Normals[54]); glVertex3fv(crater4Vertices[45]);
  glNormal3fv(crater4Normals[47]); glVertex3fv(crater4Vertices[42]);
  glNormal3fv(crater4Normals[46]); glVertex3fv(crater4Vertices[38]);
  glNormal3fv(crater4Normals[55]); glVertex3fv(crater4Vertices[49]);
  glNormal3fv(crater4Normals[53]); glVertex3fv(crater4Vertices[48]);
  glNormal3fv(crater4Normals[48]); glVertex3fv(crater4Vertices[39]);
  glNormal3fv(crater4Normals[43]); glVertex3fv(crater4Vertices[37]);
  glNormal3fv(crater4Normals[47]); glVertex3fv(crater4Vertices[42]);
  glNormal3fv(crater4Normals[54]); glVertex3fv(crater4Vertices[45]);
  glNormal3fv(crater4Normals[56]); glVertex3fv(crater4Vertices[51]);
  glNormal3fv(crater4Normals[40]); glVertex3fv(crater4Vertices[33]);
  glNormal3fv(crater4Normals[57]); glVertex3fv(crater4Vertices[50]);
  glNormal3fv(crater4Normals[55]); glVertex3fv(crater4Vertices[49]);
  glNormal3fv(crater4Normals[46]); glVertex3fv(crater4Vertices[38]);
  glNormal3fv(crater4Normals[44]); glVertex3fv(crater4Vertices[36]);
  glNormal3fv(crater4Normals[58]); glVertex3fv(crater4Vertices[52]);
  glNormal3fv(crater4Normals[59]); glVertex3fv(crater4Vertices[44]);
  glNormal3fv(crater4Normals[45]); glVertex3fv(crater4Vertices[43]);
  glNormal3fv(crater4Normals[44]); glVertex3fv(crater4Vertices[36]);
  glNormal3fv(crater4Normals[43]); glVertex3fv(crater4Vertices[37]);
  glNormal3fv(crater4Normals[56]); glVertex3fv(crater4Vertices[51]);
  glNormal3fv(crater4Normals[58]); glVertex3fv(crater4Vertices[52]);
  glNormal3fv(crater4Normals[58]); glVertex3fv(crater4Vertices[52]);
  glNormal3fv(crater4Normals[56]); glVertex3fv(crater4Vertices[51]);
  glNormal3fv(crater4Normals[60]); glVertex3fv(crater4Vertices[54]);
  glNormal3fv(crater4Normals[61]); glVertex3fv(crater4Vertices[53]);
  glNormal3fv(crater4Normals[58]); glVertex3fv(crater4Vertices[52]);
  glNormal3fv(crater4Normals[61]); glVertex3fv(crater4Vertices[53]);
  glNormal3fv(crater4Normals[62]); glVertex3fv(crater4Vertices[61]);
  glNormal3fv(crater4Normals[59]); glVertex3fv(crater4Vertices[44]);
  glNormal3fv(crater4Normals[57]); glVertex3fv(crater4Vertices[50]);
  glNormal3fv(crater4Normals[63]); glVertex3fv(crater4Vertices[55]);
  glNormal3fv(crater4Normals[64]); glVertex3fv(crater4Vertices[56]);
  glNormal3fv(crater4Normals[55]); glVertex3fv(crater4Vertices[49]);
  glNormal3fv(crater4Normals[56]); glVertex3fv(crater4Vertices[51]);
  glNormal3fv(crater4Normals[54]); glVertex3fv(crater4Vertices[45]);
  glNormal3fv(crater4Normals[65]); glVertex3fv(crater4Vertices[60]);
  glNormal3fv(crater4Normals[60]); glVertex3fv(crater4Vertices[54]);
  glNormal3fv(crater4Normals[55]); glVertex3fv(crater4Vertices[49]);
  glNormal3fv(crater4Normals[64]); glVertex3fv(crater4Vertices[56]);
  glNormal3fv(crater4Normals[66]); glVertex3fv(crater4Vertices[57]);
  glNormal3fv(crater4Normals[53]); glVertex3fv(crater4Vertices[48]);
  glNormal3fv(crater4Normals[52]); glVertex3fv(crater4Vertices[46]);
  glNormal3fv(crater4Normals[67]); glVertex3fv(crater4Vertices[59]);
  glNormal3fv(crater4Normals[65]); glVertex3fv(crater4Vertices[60]);
  glNormal3fv(crater4Normals[54]); glVertex3fv(crater4Vertices[45]);
  glNormal3fv(crater4Normals[53]); glVertex3fv(crater4Vertices[48]);
  glNormal3fv(crater4Normals[66]); glVertex3fv(crater4Vertices[57]);
  glNormal3fv(crater4Normals[68]); glVertex3fv(crater4Vertices[58]);
  glNormal3fv(crater4Normals[51]); glVertex3fv(crater4Vertices[47]);
  glNormal3fv(crater4Normals[51]); glVertex3fv(crater4Vertices[47]);
  glNormal3fv(crater4Normals[68]); glVertex3fv(crater4Vertices[58]);
  glNormal3fv(crater4Normals[67]); glVertex3fv(crater4Vertices[59]);
  glNormal3fv(crater4Normals[52]); glVertex3fv(crater4Vertices[46]);
  glNormal3fv(crater4Normals[68]); glVertex3fv(crater4Vertices[58]);
  glNormal3fv(crater4Normals[69]); glVertex3fv(crater4Vertices[65]);
  glNormal3fv(crater4Normals[70]); glVertex3fv(crater4Vertices[64]);
  glNormal3fv(crater4Normals[67]); glVertex3fv(crater4Vertices[59]);
  glNormal3fv(crater4Normals[66]); glVertex3fv(crater4Vertices[57]);
  glNormal3fv(crater4Normals[71]); glVertex3fv(crater4Vertices[66]);
  glNormal3fv(crater4Normals[69]); glVertex3fv(crater4Vertices[65]);
  glNormal3fv(crater4Normals[68]); glVertex3fv(crater4Vertices[58]);
  glNormal3fv(crater4Normals[67]); glVertex3fv(crater4Vertices[59]);
  glNormal3fv(crater4Normals[70]); glVertex3fv(crater4Vertices[64]);
  glNormal3fv(crater4Normals[72]); glVertex3fv(crater4Vertices[63]);
  glNormal3fv(crater4Normals[65]); glVertex3fv(crater4Vertices[60]);
  glNormal3fv(crater4Normals[64]); glVertex3fv(crater4Vertices[56]);
  glNormal3fv(crater4Normals[73]); glVertex3fv(crater4Vertices[67]);
  glNormal3fv(crater4Normals[71]); glVertex3fv(crater4Vertices[66]);
  glNormal3fv(crater4Normals[66]); glVertex3fv(crater4Vertices[57]);
  glNormal3fv(crater4Normals[60]); glVertex3fv(crater4Vertices[54]);
  glNormal3fv(crater4Normals[65]); glVertex3fv(crater4Vertices[60]);
  glNormal3fv(crater4Normals[72]); glVertex3fv(crater4Vertices[63]);
  glNormal3fv(crater4Normals[74]); glVertex3fv(crater4Vertices[68]);
  glNormal3fv(crater4Normals[61]); glVertex3fv(crater4Vertices[53]);
  glNormal3fv(crater4Normals[75]); glVertex3fv(crater4Vertices[69]);
  glNormal3fv(crater4Normals[76]); glVertex3fv(crater4Vertices[62]);
  glNormal3fv(crater4Normals[62]); glVertex3fv(crater4Vertices[61]);
  glNormal3fv(crater4Normals[61]); glVertex3fv(crater4Vertices[53]);
  glNormal3fv(crater4Normals[60]); glVertex3fv(crater4Vertices[54]);
  glNormal3fv(crater4Normals[74]); glVertex3fv(crater4Vertices[68]);
  glNormal3fv(crater4Normals[75]); glVertex3fv(crater4Vertices[69]);
  glNormal3fv(crater4Normals[75]); glVertex3fv(crater4Vertices[69]);
  glNormal3fv(crater4Normals[74]); glVertex3fv(crater4Vertices[68]);
  glNormal3fv(crater4Normals[77]); glVertex3fv(crater4Vertices[71]);
  glNormal3fv(crater4Normals[78]); glVertex3fv(crater4Vertices[70]);
  glNormal3fv(crater4Normals[75]); glVertex3fv(crater4Vertices[69]);
  glNormal3fv(crater4Normals[78]); glVertex3fv(crater4Vertices[70]);
  glNormal3fv(crater4Normals[79]); glVertex3fv(crater4Vertices[78]);
  glNormal3fv(crater4Normals[76]); glVertex3fv(crater4Vertices[62]);
  glNormal3fv(crater4Normals[63]); glVertex3fv(crater4Vertices[55]);
  glNormal3fv(crater4Normals[80]); glVertex3fv(crater4Vertices[72]);
  glNormal3fv(crater4Normals[81]); glVertex3fv(crater4Vertices[73]);
  glNormal3fv(crater4Normals[73]); glVertex3fv(crater4Vertices[67]);
  glNormal3fv(crater4Normals[74]); glVertex3fv(crater4Vertices[68]);
  glNormal3fv(crater4Normals[72]); glVertex3fv(crater4Vertices[63]);
  glNormal3fv(crater4Normals[82]); glVertex3fv(crater4Vertices[77]);
  glNormal3fv(crater4Normals[77]); glVertex3fv(crater4Vertices[71]);
  glNormal3fv(crater4Normals[73]); glVertex3fv(crater4Vertices[67]);
  glNormal3fv(crater4Normals[81]); glVertex3fv(crater4Vertices[73]);
  glNormal3fv(crater4Normals[83]); glVertex3fv(crater4Vertices[74]);
  glNormal3fv(crater4Normals[71]); glVertex3fv(crater4Vertices[66]);
  glNormal3fv(crater4Normals[70]); glVertex3fv(crater4Vertices[64]);
  glNormal3fv(crater4Normals[84]); glVertex3fv(crater4Vertices[76]);
  glNormal3fv(crater4Normals[82]); glVertex3fv(crater4Vertices[77]);
  glNormal3fv(crater4Normals[72]); glVertex3fv(crater4Vertices[63]);
  glNormal3fv(crater4Normals[71]); glVertex3fv(crater4Vertices[66]);
  glNormal3fv(crater4Normals[83]); glVertex3fv(crater4Vertices[74]);
  glNormal3fv(crater4Normals[85]); glVertex3fv(crater4Vertices[75]);
  glNormal3fv(crater4Normals[69]); glVertex3fv(crater4Vertices[65]);
  glNormal3fv(crater4Normals[69]); glVertex3fv(crater4Vertices[65]);
  glNormal3fv(crater4Normals[85]); glVertex3fv(crater4Vertices[75]);
  glNormal3fv(crater4Normals[84]); glVertex3fv(crater4Vertices[76]);
  glNormal3fv(crater4Normals[70]); glVertex3fv(crater4Vertices[64]);
  glNormal3fv(crater4Normals[85]); glVertex3fv(crater4Vertices[75]);
  glNormal3fv(crater4Normals[86]); glVertex3fv(crater4Vertices[82]);
  glNormal3fv(crater4Normals[87]); glVertex3fv(crater4Vertices[81]);
  glNormal3fv(crater4Normals[84]); glVertex3fv(crater4Vertices[76]);
  glNormal3fv(crater4Normals[83]); glVertex3fv(crater4Vertices[74]);
  glNormal3fv(crater4Normals[88]); glVertex3fv(crater4Vertices[83]);
  glNormal3fv(crater4Normals[86]); glVertex3fv(crater4Vertices[82]);
  glNormal3fv(crater4Normals[85]); glVertex3fv(crater4Vertices[75]);
  glNormal3fv(crater4Normals[84]); glVertex3fv(crater4Vertices[76]);
  glNormal3fv(crater4Normals[87]); glVertex3fv(crater4Vertices[81]);
  glNormal3fv(crater4Normals[89]); glVertex3fv(crater4Vertices[80]);
  glNormal3fv(crater4Normals[82]); glVertex3fv(crater4Vertices[77]);
  glNormal3fv(crater4Normals[81]); glVertex3fv(crater4Vertices[73]);
  glNormal3fv(crater4Normals[90]); glVertex3fv(crater4Vertices[84]);
  glNormal3fv(crater4Normals[88]); glVertex3fv(crater4Vertices[83]);
  glNormal3fv(crater4Normals[83]); glVertex3fv(crater4Vertices[74]);
  glNormal3fv(crater4Normals[77]); glVertex3fv(crater4Vertices[71]);
  glNormal3fv(crater4Normals[82]); glVertex3fv(crater4Vertices[77]);
  glNormal3fv(crater4Normals[89]); glVertex3fv(crater4Vertices[80]);
  glNormal3fv(crater4Normals[91]); glVertex3fv(crater4Vertices[85]);
  glNormal3fv(crater4Normals[78]); glVertex3fv(crater4Vertices[70]);
  glNormal3fv(crater4Normals[92]); glVertex3fv(crater4Vertices[86]);
  glNormal3fv(crater4Normals[93]); glVertex3fv(crater4Vertices[79]);
  glNormal3fv(crater4Normals[79]); glVertex3fv(crater4Vertices[78]);
  glNormal3fv(crater4Normals[78]); glVertex3fv(crater4Vertices[70]);
  glNormal3fv(crater4Normals[77]); glVertex3fv(crater4Vertices[71]);
  glNormal3fv(crater4Normals[91]); glVertex3fv(crater4Vertices[85]);
  glNormal3fv(crater4Normals[92]); glVertex3fv(crater4Vertices[86]);
  glNormal3fv(crater4Normals[92]); glVertex3fv(crater4Vertices[86]);
  glNormal3fv(crater4Normals[91]); glVertex3fv(crater4Vertices[85]);
  glNormal3fv(crater4Normals[94]); glVertex3fv(crater4Vertices[88]);
  glNormal3fv(crater4Normals[95]); glVertex3fv(crater4Vertices[87]);
  glNormal3fv(crater4Normals[92]); glVertex3fv(crater4Vertices[86]);
  glNormal3fv(crater4Normals[95]); glVertex3fv(crater4Vertices[87]);
  glNormal3fv(crater4Normals[96]); glVertex3fv(crater4Vertices[95]);
  glNormal3fv(crater4Normals[93]); glVertex3fv(crater4Vertices[79]);
  glNormal3fv(crater4Normals[80]); glVertex3fv(crater4Vertices[72]);
  glNormal3fv(crater4Normals[97]); glVertex3fv(crater4Vertices[89]);
  glNormal3fv(crater4Normals[98]); glVertex3fv(crater4Vertices[90]);
  glNormal3fv(crater4Normals[90]); glVertex3fv(crater4Vertices[84]);
  glNormal3fv(crater4Normals[91]); glVertex3fv(crater4Vertices[85]);
  glNormal3fv(crater4Normals[89]); glVertex3fv(crater4Vertices[80]);
  glNormal3fv(crater4Normals[99]); glVertex3fv(crater4Vertices[94]);
  glNormal3fv(crater4Normals[94]); glVertex3fv(crater4Vertices[88]);
  glNormal3fv(crater4Normals[90]); glVertex3fv(crater4Vertices[84]);
  glNormal3fv(crater4Normals[98]); glVertex3fv(crater4Vertices[90]);
  glNormal3fv(crater4Normals[100]); glVertex3fv(crater4Vertices[91]);
  glNormal3fv(crater4Normals[88]); glVertex3fv(crater4Vertices[83]);
  glNormal3fv(crater4Normals[87]); glVertex3fv(crater4Vertices[81]);
  glNormal3fv(crater4Normals[101]); glVertex3fv(crater4Vertices[93]);
  glNormal3fv(crater4Normals[99]); glVertex3fv(crater4Vertices[94]);
  glNormal3fv(crater4Normals[89]); glVertex3fv(crater4Vertices[80]);
  glNormal3fv(crater4Normals[88]); glVertex3fv(crater4Vertices[83]);
  glNormal3fv(crater4Normals[100]); glVertex3fv(crater4Vertices[91]);
  glNormal3fv(crater4Normals[102]); glVertex3fv(crater4Vertices[92]);
  glNormal3fv(crater4Normals[86]); glVertex3fv(crater4Vertices[82]);
  glNormal3fv(crater4Normals[86]); glVertex3fv(crater4Vertices[82]);
  glNormal3fv(crater4Normals[102]); glVertex3fv(crater4Vertices[92]);
  glNormal3fv(crater4Normals[101]); glVertex3fv(crater4Vertices[93]);
  glNormal3fv(crater4Normals[87]); glVertex3fv(crater4Vertices[81]);
  glNormal3fv(crater4Normals[102]); glVertex3fv(crater4Vertices[92]);
  glNormal3fv(crater4Normals[103]); glVertex3fv(crater4Vertices[99]);
  glNormal3fv(crater4Normals[104]); glVertex3fv(crater4Vertices[98]);
  glNormal3fv(crater4Normals[101]); glVertex3fv(crater4Vertices[93]);
  glNormal3fv(crater4Normals[100]); glVertex3fv(crater4Vertices[91]);
  glNormal3fv(crater4Normals[105]); glVertex3fv(crater4Vertices[100]);
  glNormal3fv(crater4Normals[103]); glVertex3fv(crater4Vertices[99]);
  glNormal3fv(crater4Normals[102]); glVertex3fv(crater4Vertices[92]);
  glNormal3fv(crater4Normals[101]); glVertex3fv(crater4Vertices[93]);
  glNormal3fv(crater4Normals[104]); glVertex3fv(crater4Vertices[98]);
  glNormal3fv(crater4Normals[106]); glVertex3fv(crater4Vertices[97]);
  glNormal3fv(crater4Normals[99]); glVertex3fv(crater4Vertices[94]);
  glNormal3fv(crater4Normals[98]); glVertex3fv(crater4Vertices[90]);
  glNormal3fv(crater4Normals[107]); glVertex3fv(crater4Vertices[101]);
  glNormal3fv(crater4Normals[105]); glVertex3fv(crater4Vertices[100]);
  glNormal3fv(crater4Normals[100]); glVertex3fv(crater4Vertices[91]);
  glNormal3fv(crater4Normals[94]); glVertex3fv(crater4Vertices[88]);
  glNormal3fv(crater4Normals[99]); glVertex3fv(crater4Vertices[94]);
  glNormal3fv(crater4Normals[106]); glVertex3fv(crater4Vertices[97]);
  glNormal3fv(crater4Normals[108]); glVertex3fv(crater4Vertices[102]);
  glNormal3fv(crater4Normals[97]); glVertex3fv(crater4Vertices[89]);
  glNormal3fv(crater4Normals[109]); glVertex3fv(crater4Vertices[106]);
  glNormal3fv(crater4Normals[107]); glVertex3fv(crater4Vertices[101]);
  glNormal3fv(crater4Normals[98]); glVertex3fv(crater4Vertices[90]);
  glNormal3fv(crater4Normals[95]); glVertex3fv(crater4Vertices[87]);
  glNormal3fv(crater4Normals[110]); glVertex3fv(crater4Vertices[103]);
  glNormal3fv(crater4Normals[111]); glVertex3fv(crater4Vertices[96]);
  glNormal3fv(crater4Normals[96]); glVertex3fv(crater4Vertices[95]);
  glNormal3fv(crater4Normals[95]); glVertex3fv(crater4Vertices[87]);
  glNormal3fv(crater4Normals[94]); glVertex3fv(crater4Vertices[88]);
  glNormal3fv(crater4Normals[108]); glVertex3fv(crater4Vertices[102]);
  glNormal3fv(crater4Normals[110]); glVertex3fv(crater4Vertices[103]);
  glNormal3fv(crater4Normals[110]); glVertex3fv(crater4Vertices[103]);
  glNormal3fv(crater4Normals[108]); glVertex3fv(crater4Vertices[102]);
  glNormal3fv(crater4Normals[112]); glVertex3fv(crater4Vertices[105]);
  glNormal3fv(crater4Normals[113]); glVertex3fv(crater4Vertices[104]);
  glNormal3fv(crater4Normals[110]); glVertex3fv(crater4Vertices[103]);
  glNormal3fv(crater4Normals[113]); glVertex3fv(crater4Vertices[104]);
  glNormal3fv(crater4Normals[114]); glVertex3fv(crater4Vertices[112]);
  glNormal3fv(crater4Normals[111]); glVertex3fv(crater4Vertices[96]);
  glNormal3fv(crater4Normals[108]); glVertex3fv(crater4Vertices[102]);
  glNormal3fv(crater4Normals[106]); glVertex3fv(crater4Vertices[97]);
  glNormal3fv(crater4Normals[116]); glVertex3fv(crater4Vertices[111]);
  glNormal3fv(crater4Normals[112]); glVertex3fv(crater4Vertices[105]);
  glNormal3fv(crater4Normals[107]); glVertex3fv(crater4Vertices[101]);
  glNormal3fv(crater4Normals[115]); glVertex3fv(crater4Vertices[107]);
  glNormal3fv(crater4Normals[117]); glVertex3fv(crater4Vertices[108]);
  glNormal3fv(crater4Normals[105]); glVertex3fv(crater4Vertices[100]);
  glNormal3fv(crater4Normals[104]); glVertex3fv(crater4Vertices[98]);
  glNormal3fv(crater4Normals[118]); glVertex3fv(crater4Vertices[110]);
  glNormal3fv(crater4Normals[116]); glVertex3fv(crater4Vertices[111]);
  glNormal3fv(crater4Normals[106]); glVertex3fv(crater4Vertices[97]);
  glNormal3fv(crater4Normals[105]); glVertex3fv(crater4Vertices[100]);
  glNormal3fv(crater4Normals[117]); glVertex3fv(crater4Vertices[108]);
  glNormal3fv(crater4Normals[119]); glVertex3fv(crater4Vertices[109]);
  glNormal3fv(crater4Normals[103]); glVertex3fv(crater4Vertices[99]);
  glNormal3fv(crater4Normals[103]); glVertex3fv(crater4Vertices[99]);
  glNormal3fv(crater4Normals[119]); glVertex3fv(crater4Vertices[109]);
  glNormal3fv(crater4Normals[118]); glVertex3fv(crater4Vertices[110]);
  glNormal3fv(crater4Normals[104]); glVertex3fv(crater4Vertices[98]);
  glNormal3fv(crater4Normals[119]); glVertex3fv(crater4Vertices[109]);
  glNormal3fv(crater4Normals[120]); glVertex3fv(crater4Vertices[116]);
  glNormal3fv(crater4Normals[121]); glVertex3fv(crater4Vertices[115]);
  glNormal3fv(crater4Normals[118]); glVertex3fv(crater4Vertices[110]);
  glNormal3fv(crater4Normals[117]); glVertex3fv(crater4Vertices[108]);
  glNormal3fv(crater4Normals[122]); glVertex3fv(crater4Vertices[117]);
  glNormal3fv(crater4Normals[120]); glVertex3fv(crater4Vertices[116]);
  glNormal3fv(crater4Normals[119]); glVertex3fv(crater4Vertices[109]);
  glNormal3fv(crater4Normals[118]); glVertex3fv(crater4Vertices[110]);
  glNormal3fv(crater4Normals[121]); glVertex3fv(crater4Vertices[115]);
  glNormal3fv(crater4Normals[123]); glVertex3fv(crater4Vertices[114]);
  glNormal3fv(crater4Normals[116]); glVertex3fv(crater4Vertices[111]);
  glNormal3fv(crater4Normals[115]); glVertex3fv(crater4Vertices[107]);
  glNormal3fv(crater4Normals[124]); glVertex3fv(crater4Vertices[118]);
  glNormal3fv(crater4Normals[122]); glVertex3fv(crater4Vertices[117]);
  glNormal3fv(crater4Normals[117]); glVertex3fv(crater4Vertices[108]);
  glNormal3fv(crater4Normals[112]); glVertex3fv(crater4Vertices[105]);
  glNormal3fv(crater4Normals[116]); glVertex3fv(crater4Vertices[111]);
  glNormal3fv(crater4Normals[123]); glVertex3fv(crater4Vertices[114]);
  glNormal3fv(crater4Normals[125]); glVertex3fv(crater4Vertices[120]);
  glNormal3fv(crater4Normals[109]); glVertex3fv(crater4Vertices[106]);
  glNormal3fv(crater4Normals[126]); glVertex3fv(crater4Vertices[119]);
  glNormal3fv(crater4Normals[124]); glVertex3fv(crater4Vertices[118]);
  glNormal3fv(crater4Normals[115]); glVertex3fv(crater4Vertices[107]);
  glNormal3fv(crater4Normals[113]); glVertex3fv(crater4Vertices[104]);
  glNormal3fv(crater4Normals[127]); glVertex3fv(crater4Vertices[121]);
  glNormal3fv(crater4Normals[128]); glVertex3fv(crater4Vertices[113]);
  glNormal3fv(crater4Normals[114]); glVertex3fv(crater4Vertices[112]);
  glNormal3fv(crater4Normals[113]); glVertex3fv(crater4Vertices[104]);
  glNormal3fv(crater4Normals[112]); glVertex3fv(crater4Vertices[105]);
  glNormal3fv(crater4Normals[125]); glVertex3fv(crater4Vertices[120]);
  glNormal3fv(crater4Normals[127]); glVertex3fv(crater4Vertices[121]);
  glNormal3fv(crater4Normals[127]); glVertex3fv(crater4Vertices[121]);
  glNormal3fv(crater4Normals[125]); glVertex3fv(crater4Vertices[120]);
  glNormal3fv(crater4Normals[129]); glVertex3fv(crater4Vertices[123]);
  glNormal3fv(crater4Normals[130]); glVertex3fv(crater4Vertices[122]);
  glNormal3fv(crater4Normals[127]); glVertex3fv(crater4Vertices[121]);
  glNormal3fv(crater4Normals[130]); glVertex3fv(crater4Vertices[122]);
  glNormal3fv(crater4Normals[131]); glVertex3fv(crater4Vertices[129]);
  glNormal3fv(crater4Normals[128]); glVertex3fv(crater4Vertices[113]);
  glNormal3fv(crater4Normals[126]); glVertex3fv(crater4Vertices[119]);
  glNormal3fv(crater4Normals[8]); glVertex3fv(crater4Vertices[1]);
  glNormal3fv(crater4Normals[132]); glVertex3fv(crater4Vertices[124]);
  glNormal3fv(crater4Normals[124]); glVertex3fv(crater4Vertices[118]);
  glNormal3fv(crater4Normals[125]); glVertex3fv(crater4Vertices[120]);
  glNormal3fv(crater4Normals[123]); glVertex3fv(crater4Vertices[114]);
  glNormal3fv(crater4Normals[133]); glVertex3fv(crater4Vertices[128]);
  glNormal3fv(crater4Normals[129]); glVertex3fv(crater4Vertices[123]);
  glNormal3fv(crater4Normals[124]); glVertex3fv(crater4Vertices[118]);
  glNormal3fv(crater4Normals[132]); glVertex3fv(crater4Vertices[124]);
  glNormal3fv(crater4Normals[134]); glVertex3fv(crater4Vertices[125]);
  glNormal3fv(crater4Normals[122]); glVertex3fv(crater4Vertices[117]);
  glNormal3fv(crater4Normals[121]); glVertex3fv(crater4Vertices[115]);
  glNormal3fv(crater4Normals[135]); glVertex3fv(crater4Vertices[127]);
  glNormal3fv(crater4Normals[133]); glVertex3fv(crater4Vertices[128]);
  glNormal3fv(crater4Normals[123]); glVertex3fv(crater4Vertices[114]);
  glNormal3fv(crater4Normals[122]); glVertex3fv(crater4Vertices[117]);
  glNormal3fv(crater4Normals[134]); glVertex3fv(crater4Vertices[125]);
  glNormal3fv(crater4Normals[136]); glVertex3fv(crater4Vertices[126]);
  glNormal3fv(crater4Normals[120]); glVertex3fv(crater4Vertices[116]);
  glNormal3fv(crater4Normals[120]); glVertex3fv(crater4Vertices[116]);
  glNormal3fv(crater4Normals[136]); glVertex3fv(crater4Vertices[126]);
  glNormal3fv(crater4Normals[135]); glVertex3fv(crater4Vertices[127]);
  glNormal3fv(crater4Normals[121]); glVertex3fv(crater4Vertices[115]);
  glNormal3fv(crater4Normals[136]); glVertex3fv(crater4Vertices[126]);
  glNormal3fv(crater4Normals[17]); glVertex3fv(crater4Vertices[133]);
  glNormal3fv(crater4Normals[14]); glVertex3fv(crater4Vertices[132]);
  glNormal3fv(crater4Normals[135]); glVertex3fv(crater4Vertices[127]);
  glNormal3fv(crater4Normals[134]); glVertex3fv(crater4Vertices[125]);
  glNormal3fv(crater4Normals[13]); glVertex3fv(crater4Vertices[134]);
  glNormal3fv(crater4Normals[17]); glVertex3fv(crater4Vertices[133]);
  glNormal3fv(crater4Normals[136]); glVertex3fv(crater4Vertices[126]);
  glNormal3fv(crater4Normals[135]); glVertex3fv(crater4Vertices[127]);
  glNormal3fv(crater4Normals[14]); glVertex3fv(crater4Vertices[132]);
  glNormal3fv(crater4Normals[10]); glVertex3fv(crater4Vertices[131]);
  glNormal3fv(crater4Normals[133]); glVertex3fv(crater4Vertices[128]);
  glNormal3fv(crater4Normals[132]); glVertex3fv(crater4Vertices[124]);
  glNormal3fv(crater4Normals[7]); glVertex3fv(crater4Vertices[0]);
  glNormal3fv(crater4Normals[13]); glVertex3fv(crater4Vertices[134]);
  glNormal3fv(crater4Normals[134]); glVertex3fv(crater4Vertices[125]);
  glNormal3fv(crater4Normals[129]); glVertex3fv(crater4Vertices[123]);
  glNormal3fv(crater4Normals[133]); glVertex3fv(crater4Vertices[128]);
  glNormal3fv(crater4Normals[10]); glVertex3fv(crater4Vertices[131]);
  glNormal3fv(crater4Normals[1]); glVertex3fv(crater4Vertices[135]);
  glNormal3fv(crater4Normals[130]); glVertex3fv(crater4Vertices[122]);
  glNormal3fv(crater4Normals[0]); glVertex3fv(crater4Vertices[136]);
  glNormal3fv(crater4Normals[5]); glVertex3fv(crater4Vertices[130]);
  glNormal3fv(crater4Normals[131]); glVertex3fv(crater4Vertices[129]);
  glNormal3fv(crater4Normals[130]); glVertex3fv(crater4Vertices[122]);
  glNormal3fv(crater4Normals[129]); glVertex3fv(crater4Vertices[123]);
  glNormal3fv(crater4Normals[1]); glVertex3fv(crater4Vertices[135]);
  glNormal3fv(crater4Normals[0]); glVertex3fv(crater4Vertices[136]);
  glEnd();

  glBegin(GL_TRIANGLES);
  glNormal3fv(crater4Normals[9]); glVertex3fv(crater4Vertices[16]);
  glNormal3fv(crater4Normals[22]); glVertex3fv(crater4Vertices[15]);
  glNormal3fv(crater4Normals[6]); glVertex3fv(crater4Vertices[4]);
  glNormal3fv(crater4Normals[9]); glVertex3fv(crater4Vertices[16]);
  glNormal3fv(crater4Normals[29]); glVertex3fv(crater4Vertices[21]);
  glNormal3fv(crater4Normals[22]); glVertex3fv(crater4Vertices[15]);
  glNormal3fv(crater4Normals[40]); glVertex3fv(crater4Vertices[33]);
  glNormal3fv(crater4Normals[46]); glVertex3fv(crater4Vertices[38]);
  glNormal3fv(crater4Normals[38]); glVertex3fv(crater4Vertices[32]);
  glNormal3fv(crater4Normals[63]); glVertex3fv(crater4Vertices[55]);
  glNormal3fv(crater4Normals[73]); glVertex3fv(crater4Vertices[67]);
  glNormal3fv(crater4Normals[64]); glVertex3fv(crater4Vertices[56]);
  glNormal3fv(crater4Normals[80]); glVertex3fv(crater4Vertices[72]);
  glNormal3fv(crater4Normals[90]); glVertex3fv(crater4Vertices[84]);
  glNormal3fv(crater4Normals[81]); glVertex3fv(crater4Vertices[73]);
  glNormal3fv(crater4Normals[109]); glVertex3fv(crater4Vertices[106]);
  glNormal3fv(crater4Normals[115]); glVertex3fv(crater4Vertices[107]);
  glNormal3fv(crater4Normals[107]); glVertex3fv(crater4Vertices[101]);
  glNormal3fv(crater4Normals[8]); glVertex3fv(crater4Vertices[1]);
  glNormal3fv(crater4Normals[7]); glVertex3fv(crater4Vertices[0]);
  glNormal3fv(crater4Normals[132]); glVertex3fv(crater4Vertices[124]);
  glEnd();
}
