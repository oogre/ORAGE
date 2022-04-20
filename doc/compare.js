const fs = require('fs').promises;


function rawToData(raw){
	return raw.split("\n").map(elem => elem.split("\t"));
}

function dataToRaw(data){
	return data.map(elem => elem.join("\t")).join("\n");
}


async function copare(){
	const beforeRaw = await fs.readFile('before.dump', 'utf8');
	const afterRaw = await fs.readFile('after.dump', 'utf8');

	const beforeData = rawToData(beforeRaw);
	const afterData = rawToData(afterRaw);

	const diffData = afterData.filter(line => {
		const idAfter =  line[0];
		const matched = beforeData.find(line => line[0] == idAfter);
		return !matched;
	});
	
	const diffRaw = dataToRaw(diffData);

	await fs.writeFile('diff.dump', diffRaw);

}

copare().then(()=>{
	console.log("done");
})