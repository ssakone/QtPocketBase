import QtQuick
import PocketBase

BaseCollection {
	id: collection

	function list(page = 0, perPage = 15, options = {}) {
		return new Promise(function (resolve, reject) {
			collection.getList(page, perPage, options).then(function (data) {
				resolve(JSON.parse(data));
			}).error(function (error) {
				reject(JSON.parse(error));
			});
		});
	}

	function sanitize(data) {
		let datas = {};
		for (let i in data) {
			datas[i] = data[i];
		}
		return datas;
	}
}
