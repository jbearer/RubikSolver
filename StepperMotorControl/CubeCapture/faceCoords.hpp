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


#define f00 {{350, 171},{418,192},{442,259},{372,244}}
#define f01 {{325,76},{392,100},{416,160},{348,138}}
#define f02 {{315,14},{368,25},{384,74},{324,52}}
#define f03 {{449,196},{512,212},{536,280},{476,268}}
#define f04 {{396,38},{441,58},{456,90},{414,77}}
#define f05 {{529,222},{574,235},{604,298},{556,286}}
#define f06 {{520,150},{543,160},{564,208},{523,196}}
#define f071 {{520,134},{527,125},{532,136},{532,139}}
#define f072 {{466,69},{486,78},{474,86},{474,85}}

#define f10 {{365,270},{431,284},{377,327},{325,314}}
#define f11 {{469,296},{522,308},{462,345},{420,335}}
#define f12 {{548,312},{601,323},{547,362},{495,355}}
#define f13 {{286,337},{356,350},{312,388},{246,377}}
#define f14 {{472,376},{523,382},{474,413},{448,403}}
#define f15 {{226,405},{288,410},{257,440},{199,434}}
#define f16 {{324,414},{368,424},{342,450},{285,446}}
#define f171 {{376,443},{380,452},{368,452},{368,451}}
#define f172 {{442,423},{458,426},{444,434},{443,434}}

#define f20 {{280,228},{326,182},{347,252},{298,298}}
#define f21 {{216,292},{256,248},{270,317},{228,355}}
#define f22 {{164,352},{200,316},{212,381},{178,416}}
#define f23 {{259,138},{303,84},{321,145},{277,200}}
#define f24 {{153,264},{180,241},{188,294},{162,322}}
#define f25 {{244,60},{280,26},{299,58},{256,110}}
#define f26 {{192,124},{228,84},{238,130},{203,150}}
#define f271 {{164,160},{176,145},{178,162},{178,161}}
#define f272 {{148,223},{162,228},{150,239},{149,239}}


const int allFaces0 [27][4][2] = {f00,f01,f02,f03,f04,f05,f06,f071,f072,
                                  f10,f11,f12,f13,f14,f15,f16,f171,f172,
                                  f20,f21,f22,f23,f24,f25,f26,f271,f272};

// Image 1 (640x480)
// Order: Clockwise starting with right face
//        Reading order, with closest corner as start
#define f30 {{443,198},{462,140},{500,216},{482,270}}
#define f31 {{468,111},{484,70},{516,136},{504,186}}
#define f32 {{494,50},{509,12},{537,80},{528,117}}
#define f33 {{496,304},{512,244},{544,307},{530,370}}
#define f34 {{540,146},{550,103},{574,162},{562,152}}
#define f35 {{542,395},{557,334},{582,389},{571,446}}
#define f36 {{562,304},{582,280},{597,308},{589,360}}
#define f371 {{586,259},{600,249},{603,249},{597,284}}
#define f372 {{586,259},{600,249},{603,249},{597,284}} // Copied, for simplicity

#define f40 {{330,230},{420,214},{461,292},{374,306}}
#define f41 {{388,333},{472,320},{510,387},{426,396}}
#define f42 {{441,417},{522,408},{554,466},{476,470}}
#define f43 {{223,251},{294,237},{338,306},{264,320}}
#define f44 {{354,438},{409,423},{440,473},{373,478}}
#define f45 {{124,270},{192,254},{235,323},{170,334}}
#define f46 {{184,358},{248,349},{280,405},{227,412}}
#define f471 {{244,434},{274,432},{264,462},{264,461}}
#define f472 {{328,479},{334,467},{342,477},{342,478}}

#define f50 {{354,150},{442,129},{418,186},{326,204}}
#define f51 {{250,172},{324,152},{296,207},{220,224}}
#define f52 {{159,197},{220,180},{185,234},{122,248}}
#define f53 {{384,80},{462,57},{440,100},{363,124}}
#define f54 {{204,136},{241,124},{230,158},{176,177}}
#define f55 {{419,24},{492,2},{478,39},{400,62}}
#define f56 {{328,51},{390,32},{372,68},{338,59}}
#define f571 {{220,113},{244,82},{253,102},{252,103}}
#define f572 {{220,113},{244,82},{253,102},{252,103}} // Copied, for simplicity

const int allFaces1 [27][4][2] = {f30,f31,f32,f33,f34,f35,f36,f371,f372,
                                  f40,f41,f42,f43,f44,f45,f46,f471,f472,
                                  f50,f51,f52,f53,f54,f55,f56,f571,f571};