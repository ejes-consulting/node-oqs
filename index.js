const stream=require('stream');
const liboqs=require('./liboqs.node');

/* leventshteinDistance:  returns the Levenshtein distance between two strings */
const levenshteinDistance=(a,b)=>{
    const m=[];

	for(let i=0;i<=b.length;i++){
        m[i]=[i];
    }

    for(let j=0;j<=a.length;j++){
        m[0][j]=j;
    }

    for(let i=1;i<=b.length;i++){
        for(let j=1;j<=a.length;j++){
            const cost=a[j-1]===b[i-1]?0:1;
            m[i][j]=Math.min(m[i-1][j]+1,m[i][j-1]+1,m[i-1][j-1]+cost);
        }
    }

    return m[b.length][a.length];
};

/* findClosestMatch:  returns the closest string in a list to a given query */
const findClosestMatch=(query, stringList)=>{
    let closestMatch=null;
    let minDistance=Number.MAX_VALUE;

    for (const str of stringList) {
        const distance=levenshteinDistance(query,str);
        if(distance<minDistance) {
            minDistance=distance;
            closestMatch=str;
        }
    }

    return closestMatch;
};

module.exports={
	/* getKEMs:  returns an array of available key encapsulation mechanisms */
	getKEMs:()=>liboqs.get_kems(),
	/* generateKEMKeys(type:string):  returns an object with privateKey and publicKey Buffers */
	generateKEMKeys:(type)=>{
		const KEM=liboqs.create_kem(findClosestMatch(type,liboqs.get_kems()));
		return liboqs.generate_keys(KEM);
	},
	/* createKEM(type:string):  Returns an object with encapsulate and decapsulate methods */
	createKEM:(type)=>{
		const KEM=liboqs.create_kem(findClosestMatch(type,liboqs.get_kems()));
		return {
			/* kem.encapsulate(publicKey:Buffer):  returns an object with cipherText:Buffer and sharedSecret:Buffer */
			encapsulate:(publicKey)=>
				liboqs.encapsulate(KEM,Buffer.from(publicKey)),
			/* kem.decapsulate(ciphertext:Buffer,privateKey:Buffer):  returns a sharedSecret Buffer */
			decapsulate:(ciphertext,privateKey)=>
				liboqs.decapsulate(KEM, Buffer.from(ciphertext), Buffer.from(privateKey)),
		};
	},
	/* getSigningMethods:  returns an array of available signature algorithms */
	getSigningMethods:()=>liboqs.get_signs(),
	/* generateSigningKeys(type:string):  returns an object with privateKey and publicKey Buffers */
	generateSigningKeys:(type)=>{
		const signer=liboqs.create_sign(findClosestMatch(type,liboqs.get_signs()));
		return liboqs.generate_signing_keys(signer);
	},
	/* createSign: returns a writable stream with update and sign methods */
	createSign:(type)=>{
		const signer=liboqs.create_sign(findClosestMatch(type,liboqs.get_signs())),
			strm=new stream.Writable();
		let message=Buffer.from('','utf8');

		strm.algorithm=type;
		/* strm.update(data:Buffer,encoding:string):  updates the message to be signed */
		strm.update=(data,encoding)=>message=Buffer.concat([message,Buffer.from(data,encoding)]),
		/* strm.sign(privateKey:Buffer,encoding:string):  returns a signature Buffer */
		strm.sign=(privateKey,encoding)=>{
			strm.writable=false;
			let signature=liboqs.sign(signer,privateKey,message);
			if(typeof encoding!=='undefined')
				signature=signature.toString(encoding);
			return signature;
		};
		/* strm._write: internal method to handle incoming data */
		strm._write=(chunk,encoding,callback)=>{
			strm.update(chunk,encoding);
			callback();
		};
		strm.writable=true;
		return strm;
	},
	/* createVerify: returns a writable stream with update and verify methods */
	createVerify:(type)=>{
		const signer=liboqs.create_sign(findClosestMatch(type,liboqs.get_signs())),
			strm=new stream.Writable();
		let message=Buffer.from('','utf8');

		strm.algorithm=type;
		/* strm.update(data:Buffer,encoding:string):  updates the message to be verified */
		strm.update=(data,encoding)=>message=Buffer.concat([message,Buffer.from(data,encoding)]),
		/* strm.verify(publicKey:Buffer,signature:Buffer,encoding:string):  returns a boolean */
		strm.verify=(publicKey,signature,encoding)=>{
			strm.writable=false;
			return liboqs.verify(signer,publicKey,Buffer.from(signature,encoding),message);
		};
		/* strm._write: internal method to handle incoming data */
		strm._write=(chunk,encoding,callback)=>{
			strm.update(chunk,encoding);
			callback();
		};
		strm.writable=true;
		return strm;
	}
};

/* eof */