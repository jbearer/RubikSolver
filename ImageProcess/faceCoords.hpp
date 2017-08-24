/**
 * faceCoords.hpp
 *
 * Simple list of all coordinates of the faces
 *
 */

// Image 0 (640x480)
// Order: (Clockwise) Upper Right (0), Bottom Right (1), Left (2)
//        Reading (L->R,U->D) int he LOCAL reference frame, so with (0,0) as the CLOSEST corner, then the corresponding orientation
//          Meaning the BR face, is L-R,U-D, but the UR face is D-U,L-R (from the global reference frame)

#include <vector>

// Camera on Stand
// (Up=0=Yellow, Back=1=Blue, Left=2=Red)
const std::vector<std::vector<int>> f00 {{368,131},{453,110},{429,167},{339,187}};
const std::vector<std::vector<int>> f01 {{260,158},{331,139},{301,194},{225,213}};
const std::vector<std::vector<int>> f02 {{167,183},{228,165},{192,218},{136,232}};
const std::vector<std::vector<int>> f03 {{401,61},{481,38},{462,82},{380,106}};
const std::vector<std::vector<int>> f04 {{215,116},{251,108},{243,138},{183,158}};
const std::vector<std::vector<int>> f05 {{432,2},{499,2},{491,18},{417,40}};
const std::vector<std::vector<int>> f06 {{339,35},{404,14},{384,45},{362,39}};
const std::vector<std::vector<int>> f07 {{257,60},{265,87},{230,97},{229,97}};
// const std::vector<std::vector<int>> f072 {{257,60},{265,87},{230,97},{229,97}}; // Copied, for simplicity

const std::vector<std::vector<int>> f10 {{140,263},{199,250},{241,310},{183,318}};
const std::vector<std::vector<int>> f11 {{227,242},{303,227},{348,298},{274,309}};
const std::vector<std::vector<int>> f12 {{343,216},{431,203},{460,275},{385,288}};
const std::vector<std::vector<int>> f13 {{204,351},{261,342},{281,386},{242,394}};
const std::vector<std::vector<int>> f14 {{414,326},{486,317},{510,369},{443,379}};
const std::vector<std::vector<int>> f15 {{259,423},{282,422},{277,443},{276,443}, {351,456},{356,463},{349,465},{348,465}};
// const std::vector<std::vector<int>> f152 {{351,456},{356,463},{349,465},{348,465}};
const std::vector<std::vector<int>> f16 {{373,429},{424,412},{447,458},{387,462}};
const std::vector<std::vector<int>> f17 {{468,411},{541,401},{564,451},{494,453}};

const std::vector<std::vector<int>> f20 {{460,182},{478,139},{507,198},{494,240}};
const std::vector<std::vector<int>> f21 {{495,99},{508,61},{536,118},{523,163}};
const std::vector<std::vector<int>> f22 {{517,28},{527,2},{552,55},{544,84}};
const std::vector<std::vector<int>> f23 {{517,289},{532,240},{557,293},{543,345}};
const std::vector<std::vector<int>> f24 {{569,97},{590,146},{578,134},{564,128}};
const std::vector<std::vector<int>> f25 {{566,380},{576,326},{596,373},{589,424}};
const std::vector<std::vector<int>> f26 {{587,292},{597,276},{609,296},{603,333}};
const std::vector<std::vector<int>> f27 {{610,247},{618,242},{615,259},{615,259}};
// const std::vector<std::vector<int>> f272 = {{610,247},{618,242},{615,259},{615,259}}; // Copied, for simplicity

const std::vector<std::vector<std::vector<int>>> allFaces0{f00,f01,f02,f03,f04,f05,f06,f07,
                                  f10,f11,f12,f13,f14,f15,f16,f17,
                                  f20,f21,f22,f23,f24,f25,f26,f27};

// Camera Through the Hole
// (Right=3=Orange, Front=4=Green, Down=5=Black)
const std::vector<std::vector<int>> f47{{350, 171},{418,192},{442,259},{372,244}};
const std::vector<std::vector<int>> f46{{325,76},{392,100},{416,160},{348,138}};
const std::vector<std::vector<int>> f45{{315,14},{368,25},{384,74},{324,52}};
const std::vector<std::vector<int>> f44{{449,196},{512,212},{536,280},{476,268}};
const std::vector<std::vector<int>> f43{{396,38},{441,58},{456,90},{414,77}};
const std::vector<std::vector<int>> f42{{529,222},{574,235},{604,298},{556,286}};
const std::vector<std::vector<int>> f41{{520,150},{543,160},{564,208},{523,196}};
const std::vector<std::vector<int>> f40{{520,134},{527,125},{532,136},{532,139} , {466,69},{486,78},{474,86},{474,85}};
// const std::vector<std::vector<int>> f402{{466,69},{486,78},{474,86},{474,85}};

const std::vector<std::vector<int>> f35{{365,270},{431,284},{377,327},{325,314}};
const std::vector<std::vector<int>> f33{{469,296},{522,308},{462,345},{420,335}};
const std::vector<std::vector<int>> f30{{548,312},{601,323},{547,362},{495,355}};
const std::vector<std::vector<int>> f36{{286,337},{356,350},{312,388},{246,377}};
const std::vector<std::vector<int>> f31{{472,376},{523,382},{474,413},{448,403}};
const std::vector<std::vector<int>> f37{{226,405},{288,410},{257,440},{199,434}};
const std::vector<std::vector<int>> f34{{324,414},{368,424},{342,450},{285,446}};
const std::vector<std::vector<int>> f32{{376,443},{380,452},{368,452},{368,451}, {442,423},{458,426},{444,434},{443,434}};
// const std::vector<std::vector<int>> f322{{442,423},{458,426},{444,434},{443,434}};

const std::vector<std::vector<int>> f52{{280,228},{326,182},{347,252},{298,298}};
const std::vector<std::vector<int>> f54{{216,292},{256,248},{270,317},{228,355}};
const std::vector<std::vector<int>> f57{{164,352},{200,316},{212,381},{178,416}};
const std::vector<std::vector<int>> f51{{259,138},{303,84},{321,145},{277,200}};
const std::vector<std::vector<int>> f56{{153,264},{180,241},{188,294},{162,322}};
const std::vector<std::vector<int>> f50{{244,60},{280,26},{299,58},{256,110}};
const std::vector<std::vector<int>> f53{{192,124},{228,84},{238,130},{203,150}};
const std::vector<std::vector<int>> f55{{164,160},{176,145},{178,162},{178,161}, {148,223},{162,228},{150,239},{149,239}};
// const std::vector<std::vector<int>> f552{{148,223},{162,228},{150,239},{149,239}}

const std::vector<std::vector<std::vector<int>>> allFaces1{f30,f31,f32,f33,f34,f35,f36,f37,
                                  f40,f41,f42,f43,f44,f45,f46,f47,
                                  f50,f51,f52,f53,f54,f55,f56,f57};

