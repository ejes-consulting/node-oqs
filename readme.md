# 
liboqs node binding

## Supported Algorithms
Algorithm Type			Scheme						Security Basis				Status
KEM						Kyber						Lattice						Standardized
						BIKE, FrodoKEM, SIKE		Code, LWE, Isogeny			Alternate
						Classic McEliece, NTRU		Code, Lattice				Alternate
Signatures				Dilithium, Falcon			Lattice						Standardized
						SPHINCS+					Hash-based					Alternate
Other/Experimental		Rainbow, qTESLA, HQC		Multivariate, Lattice		Experimental

## API

### Key Exchange

#### getKEMs
Returns an array of available key encapsulation mechanisms

###### Usage
`const oqs=require('oqs');

console.log(oqs.getKEMs());
/* should output something like this
[
  'BIKE-L1',
  'BIKE-L3',
  'BIKE-L5',
  'Classic-McEliece-348864',
  'Classic-McEliece-348864f',
  'Classic-McEliece-460896',
  'Classic-McEliece-460896f',
  'Classic-McEliece-6688128',
  'Classic-McEliece-6688128f',
  'Classic-McEliece-6960119',
  'Classic-McEliece-6960119f',
  'Classic-McEliece-8192128',
  'Classic-McEliece-8192128f',
  'HQC-128',
  'HQC-192',
  'HQC-256',
  'Kyber512',
  'Kyber768',
  'Kyber1024',
  'ML-KEM-512',
  'ML-KEM-768',
  'ML-KEM-1024',
  'sntrup761',
  'FrodoKEM-640-AES',
  'FrodoKEM-640-SHAKE',
  'FrodoKEM-976-AES',
  'FrodoKEM-976-SHAKE',
  'FrodoKEM-1344-AES',
  'FrodoKEM-1344-SHAKE'
] */
`
#### generateKEMKeys(type:string)
Generates KEM algorithm returns an object with privateKey and publicKey Buffers
###### Usage
`const oqs=require('oqs');

console.log(oqs.generateKEMKeys('BIKE-L1'));
/* should output something like this:
{
  publicKey: <Buffer 2e 60 a4 bc 5f 55 c7 2f 79 9a af fc af fb ee 92 9a 03 11 9f 20 30 98 cb c9 45 55 0a a1 db fc a4 48 ec 1c fb 79 01 33 9e 5c 06 6f 28 47 8d 66 50 14 15 ... 1491 more bytes>,
  privateKey: <Buffer 4d 11 00 00 8b 12 00 00 a5 21 00 00 a2 05 00 00 86 0a 00 00 0d 2c 00 00 86 25 00 00 83 23 00 00 3a 08 00 00 08 0c 00 00 66 0b 00 00 2a 10 00 00 ce 1d ... 5173 more bytes>
}
 */
`
#### createKEM(type:string)
Returns an object with encapsulate and decapsulate methods

##### kem.encapsulate(publicKey:Buffer)
returns an object with cipherText:Buffer and sharedSecret:Buffer

##### kem.decapsulate(ciphertext:Buffer,privateKey:Buffer):  
returns a sharedSecret Buffer

###### Usage
`const oqs=require('oqs');

const BIKEkeys=oqs.generateKEMKeys('BIKE-L1');
const KEM=oqs.createKEM('BIKE-L1');
console.log(KEM.encpasulate(BIKEkeys.publicKey));
/* should output something like this:
{
  cipherText: <Buffer 12 73 1b 4b 5f 4a e2 26 d0 61 d8 af c7 a7 56 94 bf 7f 89 20 69 79 7f 17 73 f9 76 bd 17 29 04 a9 e2 39 c5 c5 4c ae 37 eb ec bd 06 95 80 23 8d e5 ef a7 ... 1523 more bytes>,
  sharedSecret: <Buffer 3c 37 c6 4d 51 d7 f9 53 67 88 ad 67 3e 99 ff 32 22 02 ce 26 32 ab 82 4a fc f9 8c 5e 61 be 54 91>
}
*/
`
## Signing
### getSigningMethods
returns an array of available signature algorithms
###### Usage
`const oqs=require('oqs');

console.log(oqs.getSigningMethods());
/* should output something like this:
[
  'Dilithium2',
  'Dilithium3',
  'Dilithium5',
  'ML-DSA-44',
  'ML-DSA-65',
  'ML-DSA-87',
  'Falcon-512',
  'Falcon-1024',
  'Falcon-padded-512',
  'Falcon-padded-1024',
  'SPHINCS+-SHA2-128f-simple',
  'SPHINCS+-SHA2-128s-simple',
  'SPHINCS+-SHA2-192f-simple',
  'SPHINCS+-SHA2-192s-simple',
  'SPHINCS+-SHA2-256f-simple',
  'SPHINCS+-SHA2-256s-simple',
  'SPHINCS+-SHAKE-128f-simple',
  'SPHINCS+-SHAKE-128s-simple',
  'SPHINCS+-SHAKE-192f-simple',
  'SPHINCS+-SHAKE-192s-simple',
  'SPHINCS+-SHAKE-256f-simple',
  'SPHINCS+-SHAKE-256s-simple',
  'MAYO-1',
  'MAYO-2',
  'MAYO-3',
  'MAYO-5',
  'cross-rsdp-128-balanced',
  'cross-rsdp-128-fast',
  'cross-rsdp-128-small',
  'cross-rsdp-192-balanced',
  'cross-rsdp-192-fast',
  'cross-rsdp-192-small',
  'cross-rsdp-256-balanced',
  'cross-rsdp-256-fast',
  'cross-rsdp-256-small',
  'cross-rsdpg-128-balanced',
  'cross-rsdpg-128-fast',
  'cross-rsdpg-128-small',
  'cross-rsdpg-192-balanced',
  'cross-rsdpg-192-fast',
  'cross-rsdpg-192-small',
  'cross-rsdpg-256-balanced',
  'cross-rsdpg-256-fast',
  'cross-rsdpg-256-small'
]*/
`

#### generateSigningKeys(type:string)
returns an object with privateKey and publicKey Buffers
###### Usage
`const oqs=require('oqs');

console.log(oqs.generateSigningKeys('Dilithium2'));
/* should output something like this:
{
  publicKey: <Buffer 09 83 0a 15 a9 a2 92 24 18 0d 1e bc 04 a4 df f9 76 68 78 10 2d b0 8f ac 8e 9b 13 bc 11 74 8c 00 77 65 d5 12 35 1b e7 f5 ef 86 28 7a 8d 2b 86 f7 bb 3d ... 1262 more bytes>,
  privateKey: <Buffer 09 83 0a 15 a9 a2 92 24 18 0d 1e bc 04 a4 df f9 76 68 78 10 2d b0 8f ac 8e 9b 13 bc 11 74 8c 00 73 02 ec 70 1d 7c 41 84 af a9 5c f5 42 bc 8f ce ec 69 ... 2478 more bytes>
} */
`

#### createSign
returns a writable stream with update and sign methods; it's pretty close to the crypto.createSign functions
##### strm.update(data:Buffer,encoding:string)
updates the message to be signed
##### strm.sign(privateKey:Buffer,encoding:string)
returns a signature Buffer
###### Usage
`const oqs=require('oqs');
let message='Hello Bob!';
let privateKey=Buffer.from(base64PrivateKey,'base64');
let sign=oqs.createSign('Dilithium2');
// sign can be treated like a stream using sign.write
sign.write(message); // or sign.update(message);
signature=sign.sign(privateKey);
console.log('signature',signature);
/* should output something like this:
signature <Buffer 35 7f d4 78 3c 76 2b fe 97 67 fd af 8d 6d c9 98 8f 9a eb d9 7f 11 6b d9 9f be 36 dd 08 03 20 9a 6b d4 c0 60 0e b5 d5 6e 0c 93 e4 05 9a 65 c8 b2 1e 60 ... 2370 more bytes>
 */
`

#### createVerify
returns a writable stream with update and verify methods
##### strm.update(data:Buffer,encoding:string)
updates the message to be verified
##### strm.verify(publicKey:Buffer,signature:Buffer,encoding:string)
returns a boolean
`const oqs=require('oqs');
let publicKey=Buffer.from(base64PublicKey,'base64'),
	signature=Buffer.from(base64Signature,'base64');
let verify=oqs.createVerify('Dilithium2');
verify.update(message)
console.log(verify.verify(publicKey,signature))
/* assuming everything went okay, you should receive:
true
*/
`

###### eof