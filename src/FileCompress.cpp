#include "FileCompress.h"

using namespace std;

void FileCompress::Compress(const string &FilePath)
{
	/*打开文件*/
	FILE *input = fopen(FilePath.c_str(), "rb");
	if (NULL == input)
	{
		cout << FilePath << " Not Found !" << endl;
		exit(1);
	}
	/*字符信息填充*/
	FillInfo(input);

	/*修改后缀名*/
	string CompressFileName;
	GetFileName(FilePath, CompressFileName);
	CompressFileName += ".gzh";

	/*建立输出文件*/
	FILE *output = fopen(CompressFileName.c_str(), "wb");
	if (NULL == output)
	{
		cout << CompressFileName << " Can Not Be Create !" << endl;
		exit(2);
	}

	/*压缩*/
	CompressCore(input, output, FilePath);

	/*关闭文件*/
	fclose(input);
	fclose(output);
}

void FileCompress::CompressCore(FILE* src, FILE* dst, const string& FilePath)
{
	assert(NULL != src);//源文件
	assert(NULL != dst);//目标文件

	fseek(src, 0, SEEK_SET);//文件指针定位到文件开头

	unsigned char buf[_SIZE_];
	unsigned char out[_SIZE_];
	
	int out_idx = 0;
	size_t n;
	int pos = 0;
	unsigned char ch = 0;

	SaveCode(dst, FilePath);//保存编码信息到文件首部

	// 读数据
	do
	{
		// 依次取每个字节转换
		memset(buf, '\0', _SIZE_);//初始化buf
		n = fread(buf, 1, _SIZE_, src);//读1024字节的源文件中的字符到buf数组中，返回读取到的数量n
		size_t idx = 0;//当前处理字符下标
		while (idx < n)//读完源文件中每一个字符
		{
			// 转换1个字节，即1个字符（8位）
			const string& CurCode = info[buf[idx++]].code;//当前处理字符的编码
			size_t len = CurCode.length();
			size_t i_len = 0;
			while (i_len < len)
			{
				for (; pos < 8 && i_len < len; pos++)
				{
					ch <<= 1;
					if (CurCode[i_len++] == '1')
					{
						ch |= 1;//当前位置1
					}
				}

				// 先缓存到out
				if (8 == pos)
				{
					out[out_idx++] = ch;
					pos = 0;
					ch = 0;

					// 输出到文件
					if (_SIZE_ == out_idx)
					{
						fwrite(out, 1, out_idx, dst);
						out_idx = 0;
					}
				}
			} //while
		} 
	} while (n > 0);

	// 处理剩余的位
	if (8 > pos && 0 < pos)
	{
		int j = 0;
		while (j++ < 8 - pos)
			ch <<= 1;
		out[out_idx++] = ch;
	}

	// 处理剩余的字节
	if (out_idx > 0)
		fwrite(out, 1, out_idx, dst);
}

void FileCompress::UnCompress(const string &FilePath)
{
	FILE *input = fopen(FilePath.c_str(), "rb");
	if (NULL == input)
	{
		cout << FilePath << " Not Found !" << endl;
		exit(3);
	}

	// 处理头部信息
	string Postfix;
	GetHead(input, Postfix);
	
	 // 创建输出文件
	size_t begin = FilePath.find_first_of("\\");
	if (begin == string::npos)
		begin = -1;
	size_t end = FilePath.find_last_of(".");
	if (end == string::npos)
		end = FilePath.length();
	string FileName = FilePath.substr(begin + 1, end - begin - 1);
	FileName += Postfix;
	FILE *output = fopen(FileName.c_str(), "wb");
	if (NULL == output)
	{
		cout << FileName << " Can Not Open !" << endl;
		exit(4);
	}

	int i = 0;
	// 填充字符
	for (; i < 256; ++i)
	{
		info[i].ch = i;
	}
	
	CodeInfo invalid;
	invalid.cnt = 0;
	HuffmanTree<CodeInfo> hfm(info, 256, invalid);
	
	UnCompressCore(input, output, hfm.GetRoot());

	fclose(input);
	fclose(output);
}

void FileCompress::UnCompressCore(FILE *input, FILE *output, HuffmanTreeNode<CodeInfo> * pRoot)
{
	assert(NULL != input);
	assert(NULL != output);

	unsigned char ReadBuf[_SIZE_];
	unsigned char WriteBuf[_SIZE_];
	memset(WriteBuf, '\0', _SIZE_);

	size_t n;
	size_t w_idx = 0;
	size_t pos = 0;
	HuffmanTreeNode<CodeInfo> * pCur = pRoot;
	long long file_len = pRoot->_weight.cnt;
	do
	{
		memset(ReadBuf, '\0', _SIZE_);
		n = fread(ReadBuf, 1, _SIZE_, input);

		// 转换ReadBuf至WriteBuf
		size_t r_idx = 0;
		for (; r_idx < n; r_idx++)
		{
			// 转换单个字节
			unsigned char ch = ReadBuf[r_idx];
			for (; pos < 8; pos++, ch <<= 1)
			{
				if ((ch & 0x80) == 0x80)
				{
					pCur = pCur->pRight;
				}
				else
				{
					pCur = pCur->pLeft;
				}

				if (NULL == pCur->pLeft && NULL == pCur->pRight)
				{
					WriteBuf[w_idx++] = pCur->_weight.ch;
					pCur = pRoot;
					if (w_idx == _SIZE_)
					{
						fwrite(WriteBuf, 1, w_idx, output);
						memset(WriteBuf, '\0', _SIZE_);
						w_idx = 0;
					}
					file_len--;
				} // if
				if (file_len == 0)
					break;
			} // for
			if (pos == 8)
				pos = 0;

		} // for

	} while (n > 0);

	if (w_idx < _SIZE_ && w_idx > 0)
		fwrite(WriteBuf, 1, w_idx, output);
}

/*获取头部信息*/
void FileCompress::GetHead(FILE *src, string &Postfix)
{
	assert(src);

	// 获取后缀名
	unsigned char buf[_FILE_NAME_SIZE_];
	int size = _FILE_NAME_SIZE_;
	GetLine(src, buf, size);
	Postfix += (char *)buf;

	// 获取行数
	GetLine(src, buf, size);
	int line = atoi((char *)buf);

	// 获取并设置字符出现次数
	while (line--)
	{
		GetLine(src, buf, size);
		info[*buf].cnt = atoi((char *)(buf + 2));
	}
}

/*获取行*/
void FileCompress::GetLine(FILE *src, unsigned char *buf, int size)
{
	assert(src);
	
	size_t n = 0;
	int index = 0;
	while (fread(buf + index, 1, 1, src) > 0)
	{
		if (index != 0 && buf[index] == '\n')
			break;

		index++;
	}
	buf[index] = '\0';
}

/*获取文件名*/
void FileCompress::GetFileName(const string &FilePath, string &output)
{
	// test.txt
	// f:\\ab\\c\\test.txt
	
	size_t begin = FilePath.find_last_of("\\");
	if (begin == string::npos)
	{
		begin = -1;
	}
	size_t end = FilePath.find_last_of(".");
	if (end == string::npos)
	{
		end = FilePath.length();
	}
	output = FilePath.substr(begin + 1, end - begin - 1);
}

/*获取扩展名*/
void FileCompress::GetPostfixName(const string &FilePath, string &output)
{
	size_t begin = FilePath.find_last_of(".");
	if (begin != string::npos)
		output = FilePath.substr(begin, FilePath.length() - begin);
}

/*填充信息*/
void FileCompress::FillInfo(FILE *src)
{
	assert(src);

	int i = 0;
	// 填充字符
	for (; i < 256; ++i)
	{
		info[i].ch = i;
	}

	// 填充出现次数
	unsigned char buf[_SIZE_];
	size_t n;
	do
	{
		n = fread(buf, 1, _SIZE_, src);
		size_t idx = 0;
		while (idx < n)
		{
			info[buf[idx++]].cnt++;
		}
	} while (n > 0);

	// 填充编码
	CodeInfo invalid;
	invalid.cnt = 0;
	HuffmanTree<CodeInfo> hfm(info, 256, invalid);

	FillCode(hfm.GetRoot());
}

/*填充编码信息*/
void FileCompress::FillCode(const HuffmanTreeNode<CodeInfo> *pRoot)
{
	if (NULL != pRoot)//递归
	{
		FillCode(pRoot->pLeft);
		FillCode(pRoot->pRight);

		if (NULL == pRoot->pLeft && NULL == pRoot->pRight)
		{
			const HuffmanTreeNode<CodeInfo> *tmp = pRoot;
			string code;
			const HuffmanTreeNode<CodeInfo> *pParent = tmp->pParent;
			while (NULL != pParent)
			{
				if (pParent->pLeft == tmp)
				{
					code += "0";
				}
				else
				{
					code += "1";
				}
				tmp = pParent;
				pParent = tmp->pParent;
			}
			reverse(code.begin(), code.end());
			info[pRoot->_weight.ch].code = code;
		}
	}
}

/*保存编码信息到文件首部*/
void FileCompress::SaveCode(FILE *dst, const string &FilePath)
{
	// 压缩文件头格式:
	// 扩展名
	// 编码行数
	// 编码行 ...

	assert(NULL != dst);

	string output;
	GetPostfixName(FilePath, output);
	output += "\n";

	string code;
	int cnt = 0;
	unsigned char buf[33];
	for (int i = 0; i < 256; ++i)
	{
		if (info[i].cnt != 0)
		{
			cnt++;
			code += info[i].ch;
			code += ",";
			sprintf((char *)buf, "%lld", info[i].cnt);
			//_itoa((int)(info[i].cnt), (char *)buf, 10);
			code += (char *)buf;	
			code += "\n";
		}
	}
	
	sprintf((char *)buf, "%d", cnt);
	//_itoa(cnt, (char *)buf, 10);
	output += (char *)buf;		
	output += "\n";
	output += code;

	fwrite(output.c_str(), 1, output.size(), dst);
}

