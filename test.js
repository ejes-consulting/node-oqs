/* test.js:  */
const liboqs = require('./liboqs.node');

const oqs=require('./index.js');

const testLIBOQSBinding = () => {
	console.log('LIBOQS binding test');

	const availableKEMs = liboqs.get_kems();
	console.log('Available KEMs:', availableKEMs);

	availableKEMs.forEach((kem)=>{
		console.log('Testing KEM:', kem);

		const kemInstance = liboqs.create_kem(kem);
		console.log('KEM instance created:',kemInstance);

		const kemKeys = liboqs.generate_keys(kemInstance);
		console.log('Generated KEM keys:', kemKeys);

		const encapsulationResult = liboqs.encapsulate(kemInstance, kemKeys.publicKey);
		console.log('Encapsulation result:', encapsulationResult);

		const sharedSecret = liboqs.decapsulate(kemInstance, encapsulationResult.cipherText, kemKeys.privateKey);
		console.log('Decapsulated shared secret:', sharedSecret);

		const availableSignatureAlgorithms = liboqs.get_signs();
		console.log('Available signature algorithms:', availableSignatureAlgorithms);

		const signatureInstance = liboqs.create_sign(availableSignatureAlgorithms[0]);
		console.log('Signature algorithm instance created:', signatureInstance);

		const signingKeys = liboqs.generate_signing_keys(signatureInstance);
		console.log('Generated signing keys:', signingKeys);

		const message = Buffer.from('hello world');
		console.log('Message to be signed:', message.toString());

		const signature = liboqs.sign(signatureInstance, signingKeys.privateKey, message);
		console.log('Generated signature:', signature.toString('hex'));

		const verificationResult = liboqs.verify(signatureInstance, signingKeys.publicKey, signature, message);
		console.log('Signature verification result:', verificationResult ? 'Valid' : 'Invalid');
	});
};

testLIBOQSBinding();

const testLIBOQSNodeBinding = () => {
	console.log('LIBOQS Node binding test');

	const testKEMs=()=>{
		let availableKEMs=oqs.getKEMs();
		console.log('avalableKEMs',availableKEMs);
	
		availableKEMs.forEach((kem)=>{
			console.log('kem', kem);
	
			let KEMKeys=oqs.generateKEMKeys(kem);
			console.log('KEMKeys',KEMKeys);
	
			let KEM=oqs.createKEM(kem);
			let capsule=KEM.encapsulate(KEMKeys.publicKey);
			console.log('capsule',capsule);
			let secret=KEM.decapsulate(capsule.cipherText,KEMKeys.privateKey);
			console.log('secret',secret);
		});	
	};

	testKEMs();

	const testSigns=()=>{
		const message='hello world';
		let availableSigns=oqs.getSigningMethods();
		console.log('availableSigns',availableSigns);

		availableSigns.forEach((signer)=>{
			console.log('signer',signer);

			let signingKeys=oqs.generateSigningKeys(signer);
			console.log('signingKeys',signingKeys);

			let sign=oqs.createSign(signer);
			sign.write(message);
			let signature=sign.sign(signingKeys.privateKey,'base64');
			console.log('signature',signature);

			let verify=oqs.createVerify(signer);
			verify.write(message);
			let authentic=verify.verify(signingKeys.publicKey,signature,'base64');	
			console.log('authentic',authentic);
		});
	};

	testSigns();
};

testLIBOQSNodeBinding();
/* eof */