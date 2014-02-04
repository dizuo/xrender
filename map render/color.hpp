// @module:		map-render/color
// @file:		color.hpp
// @author:		peteryfren
// @date:		2013/3/7
// @version:	1.0

#ifndef COLOR4_201303072046_H
#define COLOR4_201303072046_H

template<class Type>
class Color4
{
public:
	Color4()
	{
		set_value(0,0,0,0);
	}

	Color4(Type a_0, Type a_1, Type a_2, Type a_3)
	{
		set_value(a_0, a_1, a_2, a_3);
	}

	Color4(const Color4& c4)
	{
		set_value(c4.red(), c4.green(), c4.blue(), c4.alpha());
	}

	const Type& red() const { return _buf[0]; }
	const Type& green() const { return _buf[1]; }	
	const Type& blue() const { return _buf[2]; }
	const Type& alpha() const { return _buf[3]; }

	const Type* data() const { return &_buf[0]; }

	Type* data() { return &_buf[0]; }

	void set_value( Type(&c)[4] )
	{
		_buf[0] = c[0];
		_buf[1] = c[1];
		_buf[2] = c[2];
		_buf[3] = c[3];
	}

	void set_value(Type a_0, Type a_1, Type a_2, Type a_3)
	{
		_buf[0] = a_0; _buf[1] = a_1;
		_buf[2] = a_2; _buf[3] = a_3;
	}

	Color4<Type>& operator= (const Color4<Type>& c4)
	{
		set_value( c4.red(), c4.green(), c4.blue(), c4.alpha() );
		return *this;
	}

	friend bool operator ==(const Color4<Type> & v1, const Color4<Type> & v2)
	{ 
		return (v1._buf[0]==v2._buf[0] && 
			v1._buf[1]==v2._buf[1] && 
			v1._buf[2]==v2._buf[2] && 
			v1._buf[3]==v2._buf[3]); 
	}

	friend bool operator !=(const Color4<Type> & v1, const Color4<Type> & v2)
	{ 
		return !(v1 == v2); 
	}

private:

	Type _buf[4];
};

typedef Color4<float> Color4f;
typedef Color4<unsigned char> Color4ub;

inline Color4ub int_2_color4ub(int value)
{
	unsigned char red = 0xff & (value>>24);
	unsigned char green = 0xff & (value>>16);
	unsigned char blue = 0xff & (value>>8);
	unsigned char alpha = 0xff & (value);

	return Color4ub(red, green, blue, alpha);
}

inline Color4f colorub_2_color4f(const Color4ub& c4ub)
{
	return Color4f( c4ub.red()/255.0f,
		c4ub.green()/255.0f,
		c4ub.blue()/255.0f,
		c4ub.alpha()/255.0f );
}

#endif // COLOR4_201303072046_H