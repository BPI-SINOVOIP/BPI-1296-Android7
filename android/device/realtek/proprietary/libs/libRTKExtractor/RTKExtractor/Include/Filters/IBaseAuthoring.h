
#ifndef __IBASEAUTHORING_H_INCLUDED
#define __IBASEAUTHORING_H_INCLUDED

class IBaseAuthoring
{
public:
	virtual ~IBaseAuthoring() {};

	virtual void Serialize() = 0;
	virtual void Deserialize() = 0;
	virtual HRESULT Initialize(const char *lpctstrDVDPath) = 0;
	virtual HRESULT Open(const char *lpctstrDVDPath) = 0;
	virtual HRESULT Realize() = 0;
	virtual HRESULT Finalize() = 0;
};

#endif //__IBASEAUTHORING_H_INCLUDED

