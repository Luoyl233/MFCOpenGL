#pragma once


template <class T>
class MiniMatrix
{
public:
	MiniMatrix(int r, int c);
	virtual ~MiniMatrix();
	MiniMatrix(const MiniMatrix<T> &m);		//拷贝构造
	MiniMatrix<T> &operator= (const MiniMatrix<T> &m);	//赋值


	int Rows() const { return rows; }
	int Cols() const { return cols; }

	void setMatrix(T *data);
	void setMatrix(T *data, int size);

	T *const operator[] (int index);
	T const *const operator[] (int index) const;
	MiniMatrix<T> operator+ (const MiniMatrix<T> &m);
	MiniMatrix<T> operator- (const MiniMatrix<T> &m);
	MiniMatrix<T> operator* (const MiniMatrix<T> &m);
	MiniMatrix<T> operator- ();
	void print() const;

private:
	T *mem;
	int rows, cols;
};

template<class T>
MiniMatrix<T>::MiniMatrix(int r, int c)
{
	if (r < 1 || c < 1) {
		exit(-1);
	}
	rows = r; cols = c;
	mem = new T[rows * cols];
	if (mem == NULL)	exit(-1);

	memset(mem, 0, sizeof(T) * rows * cols);
}

template<class T>
MiniMatrix<T>::~MiniMatrix()
{
	delete mem;
	mem = NULL;
}

template<class T>
MiniMatrix<T>::MiniMatrix(const MiniMatrix<T> & m)
{
	rows = m.rows;	cols = m.cols;
	mem = new T[rows * cols];
	if (mem == NULL)	exit(-1);
	memcpy(mem, m.mem, rows * cols * sizeof(T));
}

template<class T>
MiniMatrix<T> & MiniMatrix<T>::operator= (const MiniMatrix<T> & m)
{
	if (&m == this)	return *this;
	if (this->mem != NULL)	delete []this->mem;
	rows = m.rows;	cols = m.cols;
	mem = new T[rows * cols];
	memcpy(mem, m.mem, rows * cols * sizeof(T));
}

template<class T>
void MiniMatrix<T>::setMatrix(T * data)
{
	for (int i = 0; i < rows * cols; i++)
		mem[i] = data[i];
}

template<class T>
void MiniMatrix<T>::setMatrix(T * data, int size)
{
}

template<class T>
T * const MiniMatrix<T>::operator[](int index)
{
	return mem + index * cols;
}

template<class T>
T const * const MiniMatrix<T>::operator[](int index) const
{
	return mem + index * cols;
}

template<class T>
MiniMatrix<T> MiniMatrix<T>::operator+ (const MiniMatrix<T> & m)
{
	if (rows != m.rows || cols != m.cols) {
		exit(-1);
	}
	MiniMatrix<T> result(rows, cols);
	for (int i = 0; i < rows * cols; i++)
		result.mem[i] = mem[i] + m.mem[i];
	return result;
}

template<class T>
MiniMatrix<T> MiniMatrix<T>::operator- (const MiniMatrix<T> & m)
{
	if (rows != m.rows || cols != m.cols) {
		exit(-1);
	}
	MiniMatrix<T> result(rows, cols);
	for (int i = 0; i < rows * cols; i++)
		result.mem[i] = mem[i] - m.mem[i];
	return result;
}

template<class T>
MiniMatrix<T> MiniMatrix<T>::operator* (const MiniMatrix<T> & m)
{
	if (cols != m.rows) {
		exit(-1);
	}
	MiniMatrix<T> result(rows, m.cols);
	for (int i = 0; i < result.rows; i++)
		for (int j = 0; j < result.cols; j++) {
			result[i][j] = 0;
			for (int k = 0; k < cols; k++)
				result[i][j] += (*this)[i][k] * m[k][j];
		}
	return result;
}

template<class T>
MiniMatrix<T> MiniMatrix<T>::operator- ()
{
	MiniMatrix<T> result = *this;
	for (int i = 0; i < rows*cols; i++)
		result.mem[i] = -result.mem[i];
	return result;
}

template<class T>
void MiniMatrix<T>::print() const
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << (*this)[i][j] << " ";
		}
		cout << endl;
	}
}


//初始化为一个平移矩阵
template <class T>
class TMatrix :public MiniMatrix<T>
{
public:
	TMatrix(int x, int y, int z) :MiniMatrix<T>(4, 4) {
		(*this)[0][0] = 1;
		(*this)[1][1] = 1;
		(*this)[2][2] = 1;
		(*this)[3][3] = 1;
		(*this)[3][0] = x;
		(*this)[3][1] = y;
		(*this)[3][2] = z;
	}
};


//缩放矩阵
template <class T>
class SMatrix :public MiniMatrix<T>
{
public:
	SMatrix(T sx, T sy, T sz) :MiniMatrix<T>(4, 4) {
		(*this)[0][0] = sx;
		(*this)[1][1] = sy;
		(*this)[2][2] = sz;
		(*this)[3][3] = 1;
	}
};


//绕z的旋转矩阵
template <class T>
class RzMatrix :public MiniMatrix<T>
{
public:
	RzMatrix(double theta) :MiniMatrix<T>(4, 4) {
		(*this)[0][0] = cos(theta);
		(*this)[1][1] = (*this)[0][0];
		(*this)[0][1] = sin(theta);
		(*this)[1][0] = -(*this)[0][1];
		(*this)[2][2] = 1;
		(*this)[3][3] = 1;
	}
};

//绕x的旋转矩阵
template <class T>
class RxMatrix :public MiniMatrix<T>
{
public:
	RxMatrix(double theta) :MiniMatrix<T>(4, 4) {
		(*this)[0][0] = 1;
		(*this)[1][1] = cos(theta);
		(*this)[2][2] = (*this)[1][1];
		(*this)[1][2] = sin(theta);
		(*this)[2][1] = -(*this)[1][2];
		(*this)[3][3] = 1;
		
	}
};


//绕z的旋转矩阵
template <class T>
class RyMatrix :public MiniMatrix<T>
{
public:
	RyMatrix(double theta) :MiniMatrix<T>(4, 4) {
		(*this)[0][0] = cos(theta);
		(*this)[1][1] = 1;
		(*this)[2][2] = (*this)[0][0];
		(*this)[3][3] = 1;
		(*this)[2][0] = sin(theta);
		(*this)[0][2] = -(*this)[2][0];
	}
};


//坐标点矩阵
template <class T>
class PMatrix :public MiniMatrix<T>
{
public:
	PMatrix(T x, T y, T z) :MiniMatrix<T>(1, 4) {
		(*this)[0][0] = x;
		(*this)[0][1] = y;
		(*this)[0][2] = z;
		(*this)[0][3] = 1;
	}
};


//斜平行投影变换矩阵
template <class T>
class LMatrix :public MiniMatrix<T>
{
public:
	LMatrix(double L, double theta) :MiniMatrix<T>(4, 4) {
		(*this)[0][0] = 1;
		(*this)[1][1] = 1;
		(*this)[2][2] = 1;
		(*this)[3][3] = 1;
		(*this)[2][0] = L * cos(theta);
		(*this)[2][1] = L * sin(theta);
	}
};