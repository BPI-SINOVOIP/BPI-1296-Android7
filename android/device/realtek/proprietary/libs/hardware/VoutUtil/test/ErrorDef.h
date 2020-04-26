#ifndef _TEST_ERROR_DEF_H_
#define _TEST_ERROR_DEF_H_

#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

const int E_NO_CREATE_VOUT_UTIL = 0x8000000;

#endif /* _TEST_ERROR_DEF_H_ */
