#include "stdafx.h"
#include "ImageManager.h"

ImageManager::ImageManager()
	: mUiAtlas(nullptr)
{
	mUiAtlas = new map<string, uiAtlas>;
}

ImageManager::~ImageManager()
{
	this->release();
}

void ImageManager::release()
{
	for (auto& it : mResourece)
	{
		if (it.second != nullptr)
		{
			SAFE_REL(it.second->texture);
			SAFE_DEL(it.second);
		}
	}

	mUiAtlas->clear();
	SAFE_DELETE(mUiAtlas);
}

void ImageManager::InsertImageFile(string key, const char * _path)
{
	if (!mResourece.count(key))
	{
		ImageResource* info = new ImageResource;

		info->memory = nullptr;
		info->fileSize = 0;

		ifstream file;
		file.open(_path, ios::in | ios::binary);

		if (file.is_open())
		{
			char buf[10] = {};
			file.seekg(0);
			file.read(buf, 10);

			if (buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G')
				info->type = PNG;
			else if (buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F')
				info->type = JPG;
			else
				info->type = BMP;

			switch (info->type)
			{
			case PNG:
			{
				BYTE _file[25] = {};
				file.seekg(0);
				file.read((char*)_file, 25);

				info->Width = (_file[16] << 24) + (_file[17] << 16) + (_file[18] << 8) + (_file[19] << 0);
				info->Height = (_file[20] << 24) + (_file[21] << 16) + (_file[22] << 8) + (_file[23] << 0);
			}
			break;

			case JPG:
			{
				BYTE _file[256] = {};
				file.seekg(0);
				file.read((char*)_file, 256);

				int i = 4;
				while (i < file.tellg()) {
					i++;
					if (_file[i] == 0xFF) {
						if (_file[i + 1] == 0xC0) {
							info->Height = _file[i + 5] * 256 + _file[i + 6];
							info->Width = _file[i + 7] * 256 + _file[i + 8];
							break;
						}
					}
				}
			}
			break;

			case BMP:
			{
				BYTE _file[24] = {};
				file.seekg(0);
				file.read((char*)_file, 24);

				info->Width = (_file[19] * 256) + _file[18];
				info->Height = (_file[23] * 256) + _file[22];

				info->Height = info->Height < 0 ? -1 * info->Height : info->Height;
			}
			break;
			}
		}

		file.close();

		locker.lock();
		HRESULT hr;
		D3DX11CreateShaderResourceViewFromFileA(Device, _path, NULL, NULL,
			&info->texture, &hr);
		assert(SUCCEEDED(hr));

		mResourece.insert(make_pair(key, info));
		locker.unlock();
	}
}

void ImageManager::InsertImageBinary(string key, string _path, SINT _frameX, SINT _frameY)
{
	if (!mResourece.count(key))
	{
		ImageResource* info = new ImageResource;

		auto THREADPOOL = LOADMANAGER->pGetThreadPool();

		THREADPOOL->ClearBeforeStart();

		FILE* f = fopen(_path.c_str(), "rb");
		string fileName;
		int count = 0;
		fseek(f, 0, SEEK_END);
		int size = ftell(f) - 32;
		fseek(f, 0, SEEK_SET);

		THREADPOOL->ResizeBeforeStart(size);

		//Tokenize File Name
		{
			fileName = _path;

			if (fileName.find_last_of("/") != string::npos)
				fileName.erase(0, fileName.find_last_of("/") + 1);


			if (fileName.find_last_of("\\") != string::npos)
				fileName.erase(0, fileName.find_last_of("\\") + 1);


			if (fileName.find_last_of(".") != string::npos)
				fileName.erase(fileName.find_last_of("."), fileName.size());
		}

		if (NULL != f)
		{
			bool hashing_Vec = false;
			BYTE buffer = 0;

			//InitVector Insert
			fread(&THREADPOOL->InitVec[0], sizeof(BYTE), 32, f);

			// 512 Byte 분할 읽기
			int i = 0;
			while (true)
			{
				fread(&THREADPOOL->CipByte[i], sizeof(BYTE), 512, f);
				i += 512;

				if (i > size - 1)
					break;
			}

			for (auto& it : THREADPOOL->InitVec)
			{
				it ^= fileName[fileName.size() - 1];
				it ^= fileName[0];
			}

		}
		fclose(f);

		int onceSize = THREADPOOL->CipByte.size() * 0.03125f;
		int leftSize = THREADPOOL->CipByte.size() - (onceSize * 32);
		int lastIndex = onceSize * 32;

		THREADPOOL->getFileHeight() = onceSize;
		THREADPOOL->Cluck_AllThread();

		while (!THREADPOOL->isAllOver())
			this_thread::yield();

		for (int i = 0; i < leftSize; ++i)
		{
			BYTE value = THREADPOOL->CipByte[lastIndex + i] ^ THREADPOOL->InitVec[(lastIndex + i) % 32];
			value ^= 255;
			THREADPOOL->OrigByte[lastIndex + i] = value;
		}

		locker.lock();

		auto vBuffer = THREADPOOL->OrigByte;

		//Read MetaData
		{
			if (vBuffer[1] == 'P' && vBuffer[2] == 'N' && vBuffer[3] == 'G')
				info->type = PNG;
			else if (vBuffer[6] == 'J' && vBuffer[7] == 'F' && vBuffer[8] == 'I' && vBuffer[9] == 'F')
				info->type = JPG;
			else
				info->type = BMP;

			switch (info->type)
			{
			case PNG:
			{
				info->Width = ((BYTE)vBuffer[16] << 24) + ((BYTE)vBuffer[17] << 16) + ((BYTE)vBuffer[18] << 8) + ((BYTE)vBuffer[19] << 0);
				info->Height = ((BYTE)vBuffer[20] << 24) + ((BYTE)vBuffer[21] << 16) + ((BYTE)vBuffer[22] << 8) + ((BYTE)vBuffer[23] << 0);
			}
			break;

			case JPG:
			{
				int i = 4;
				while (i < vBuffer.size() - 1)
				{
					i++;
					if (vBuffer[i] == (0xFF))
					{
						if (vBuffer[i + 1] == (0xC0))
						{
							info->Height = (BYTE)vBuffer[i + 5] * 256 + (BYTE)vBuffer[i + 6];
							info->Width = (BYTE)vBuffer[i + 7] * 256 + (BYTE)vBuffer[i + 8];
							break;
						}

						else if (vBuffer[i + 1] == (0xC2))
						{
							info->Height = (BYTE)vBuffer[i + 5] * 256 + (BYTE)vBuffer[i + 6];
							info->Width = (BYTE)vBuffer[i + 7] * 256 + (BYTE)vBuffer[i + 8];
							break;
						}
					}
				}
			}
			break;

			case BMP:
			{
				info->Width = ((BYTE)vBuffer[19] * 256) + (BYTE)vBuffer[18];
				info->Height = ((BYTE)vBuffer[23] * 256) + (BYTE)vBuffer[22];

				info->Height = info->Height < 0 ? -1 * info->Height : info->Height;
			}
			break;
			}
		}


		//	ImageInfo Checker
		info->fileName = fileName;
		info->Frame.x = _frameX;
		info->Frame.y = _frameY;

		if (_frameX != 0)
			info->perImageSize.x = static_cast<FLOAT>(1.0f / static_cast<FLOAT>(info->Frame.x));

		if (_frameY != 0)
			info->perImageSize.y = static_cast<FLOAT>(1.0f / static_cast<FLOAT>(info->Frame.y));

		HRESULT hr;
		D3DX11_IMAGE_LOAD_INFO LoadInfo;
		LoadInfo.Width = info->Width;
		LoadInfo.Height = info->Height;
		LoadInfo.Usage = D3D11_USAGE_DEFAULT;
		LoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		LoadInfo.CpuAccessFlags = 0;

		info->memory = &vBuffer[0];
		info->fileSize = vBuffer.size();

		D3DX11CreateShaderResourceViewFromMemory(Device, info->memory, info->fileSize, &LoadInfo, NULL, &info->texture, &hr);
		assert(SUCCEEDED(hr));

		mResourece.insert(make_pair(key, info));

		locker.unlock();
	}
}

void ImageManager::InsertImageBinary(ThreadPool * _trdPol, string key, string _path)
{
	if (_trdPol != nullptr)
	{
		if (!mResourece.count(key))
		{
			ImageResource* info = new ImageResource;

			_trdPol->ClearBeforeStart();

			FILE* f = fopen(_path.c_str(), "rb");
			string fileName;
			int count = 0;
			fseek(f, 0, SEEK_END);
			int size = ftell(f) - 32;
			fseek(f, 0, SEEK_SET);

			_trdPol->ResizeBeforeStart(size);

			//Tokenize File Name
			{
				fileName = _path;

				if (fileName.find_last_of("/") != string::npos)
					fileName.erase(0, fileName.find_last_of("/") + 1);


				if (fileName.find_last_of("\\") != string::npos)
					fileName.erase(0, fileName.find_last_of("\\") + 1);


				if (fileName.find_last_of(".") != string::npos)
					fileName.erase(fileName.find_last_of("."), fileName.size());
			}

			if (NULL != f)
			{
				bool hashing_Vec = false;
				BYTE buffer = 0;

				//InitVector Insert
				fread(&_trdPol->InitVec[0], sizeof(BYTE), 32, f);

				// 512 Byte 분할 읽기
				int i = 0;
				while (true)
				{
					fread(&_trdPol->CipByte[i], sizeof(BYTE), 512, f);
					i += 512;

					if (i > size - 1)
						break;
				}

				for (auto& it : _trdPol->InitVec)
				{
					it ^= fileName[fileName.size() - 1];
					it ^= fileName[0];
				}

			}
			fclose(f);

			int onceSize = _trdPol->CipByte.size() * 0.03125f;
			int leftSize = _trdPol->CipByte.size() - (onceSize * 32);
			int lastIndex = onceSize * 32;

			_trdPol->getFileHeight() = onceSize;
			_trdPol->Cluck_AllThread();

			while (!_trdPol->isAllOver())
				this_thread::yield();

			for (int i = 0; i < leftSize; ++i)
			{
				BYTE value = _trdPol->CipByte[lastIndex + i] ^ _trdPol->InitVec[(lastIndex + i) % 32];
				value ^= 255;
				_trdPol->OrigByte[lastIndex + i] = value;
			}

			locker.lock();

			auto vBuffer = _trdPol->OrigByte;

			//Read MetaData
			{
				if (vBuffer[1] == 'P' && vBuffer[2] == 'N' && vBuffer[3] == 'G')
					info->type = PNG;
				else if (vBuffer[6] == 'J' && vBuffer[7] == 'F' && vBuffer[8] == 'I' && vBuffer[9] == 'F')
					info->type = JPG;
				else
					info->type = BMP;

				switch (info->type)
				{
				case PNG:
				{
					info->Width = ((BYTE)vBuffer[16] << 24) + ((BYTE)vBuffer[17] << 16) + ((BYTE)vBuffer[18] << 8) + ((BYTE)vBuffer[19] << 0);
					info->Height = ((BYTE)vBuffer[20] << 24) + ((BYTE)vBuffer[21] << 16) + ((BYTE)vBuffer[22] << 8) + ((BYTE)vBuffer[23] << 0);
				}
				break;

				case JPG:
				{
					int i = 4;
					while (i < vBuffer.size() - 1)
					{
						i++;
						if (vBuffer[i] == (0xFF))
						{
							if (vBuffer[i + 1] == (0xC0))
							{
								info->Height = (BYTE)vBuffer[i + 5] * 256 + (BYTE)vBuffer[i + 6];
								info->Width = (BYTE)vBuffer[i + 7] * 256 + (BYTE)vBuffer[i + 8];
								break;
							}

							else if (vBuffer[i + 1] == (0xC2))
							{
								info->Height = (BYTE)vBuffer[i + 5] * 256 + (BYTE)vBuffer[i + 6];
								info->Width = (BYTE)vBuffer[i + 7] * 256 + (BYTE)vBuffer[i + 8];
								break;
							}
						}
					}
				}
				break;

				case BMP:
				{
					info->Width = ((BYTE)vBuffer[19] * 256) +  (BYTE)vBuffer[18];
					info->Height = ((BYTE)vBuffer[23] * 256) + (BYTE)vBuffer[22];

					info->Height = info->Height < 0 ? -1 * info->Height : info->Height;
				}
				break;
				}
			}


			HRESULT hr;
			D3DX11_IMAGE_LOAD_INFO LoadInfo;
			LoadInfo.Width = info->Width;
			LoadInfo.Height = info->Height;
			LoadInfo.Usage = D3D11_USAGE_DEFAULT;
			LoadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			LoadInfo.CpuAccessFlags = 0;

			info->memory = &vBuffer[0];
			info->fileSize = vBuffer.size();

			D3DX11CreateShaderResourceViewFromMemory(Device, info->memory, info->fileSize, &LoadInfo, NULL, &info->texture, &hr);
			assert(SUCCEEDED(hr));

			mResourece.insert(make_pair(key, info));

			locker.unlock();
		}
	}
}

void ImageManager::ReleaseMemory(string key)
{
	if (mResourece.count(key))
	{
		auto it = mResourece.find(key);

		if (it->second != nullptr)
		{
			SAFE_REL(it->second->texture);
			SAFE_DEL(it->second);
		}

		mResourece.erase(it);
	}
}
