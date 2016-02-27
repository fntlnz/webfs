/*
 * GistTestUtil.h
 *
 *  Created on: Feb 27, 2016
 *      Author: wise
 */

#ifndef TEST_STORAGE_GIST_GISTTESTUTIL_H_
#define TEST_STORAGE_GIST_GISTTESTUTIL_H_

#include "storage/storage.h"

using pStorageId = webfs::storage::Storage::pStorageId;

class TestStorageId : public webfs::storage::Storage::StorageId{
	std::string mId;
public:

	TestStorageId(const std::string &id):mId(id){}

	std::string getRemoteWriteUrl() const override{
		return mId;
	}
	std::string getRemoteReadUrl() const override{
		return mId;
	}
	std::string getRemoteEditUrl() const override{
		return mId;
	};
	std::string getRemoteRemoveUrl() const override{
		return mId;
	};

};



#endif /* TEST_STORAGE_GIST_GISTTESTUTIL_H_ */
