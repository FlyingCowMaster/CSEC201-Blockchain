#include "hash.h"

unsigned char* SSHA(const unsigned char* msg, size_t length) {
    unsigned char A, B, C, D, E; //Initial Seed Value
    A = 56;
    B = 99;
    C = 102;
    D = 67;
    E = 76;

    for (int i = 0; i < length; i++) {
        for (int round = 0; round < 8; round++) {
            unsigned char g = (B & C) | (C & D);
            unsigned char old_A = A;
            A = A >> 2;
            B = B >> 1;
            E = (g + msg[i] + B);
            D = A ^ B;
            C = (A + E);
            A = E;
            B = old_A;
        }
      
    }
    unsigned char* digest = (unsigned char*)malloc(DIGEST_SIZE * sizeof(unsigned char));
    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    digest[4] = D;
    return digest;
}


int digest_equal(struct Digest digest1, struct Digest digest2) {
    return ((digest1.hash0 == digest2.hash0) &&
        (digest1.hash1 == digest2.hash1) &&
        (digest1.hash2 == digest2.hash2) &&
        (digest1.hash3 == digest2.hash3) &&
        (digest1.hash4 == digest2.hash4));
    
}

void printDigest(struct Digest digest) {
    printf("%d %d %d %d %d\n", digest.hash0, digest.hash1, digest.hash2, digest.hash3, digest.hash4);
}

typedef struct {
    uint32_t A, B, C, D, E;
} SSHA2_state;

SSHA2_state SSHA2(const unsigned char* msg, size_t length)
{
    /* Initial state (SHA-1 IVs; the diagram doesn't specify one) */
    uint32_t A = 0x67452301;
    uint32_t B = 0xEFCDAB89;
    uint32_t C = 0x98BADCFE;
    uint32_t D = 0x10325476;
    uint32_t E = 0xC3D2E1F0;

    for (size_t i = 0; i < length; i++) {
        uint32_t b_shr2 = B >> 2;                     /* >>2 box */
        uint32_t b_shr1 = B >> 1;                     /* >>1 box */
        uint32_t maj    = (B & C) | (C & D);          /* & , & , | boxes */

        uint32_t newA = E;                            /* E wraps to A */
        uint32_t newB = A;                            /* A moves to B */
        uint32_t newC = b_shr2 + E;                   /* lower-left + */
        uint32_t newD = b_shr2 ^ b_shr1;              /* ^ box */
        uint32_t newE = b_shr1 + maj + msg[i];        /* right-hand + */

        A = newA;
        B = newB;
        C = newC;
        D = newD;
        E = newE;
    }

    SSHA2_state out = { A, B, C, D, E };
    return out;
}
