#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
using namespace std;


template<class C>class String{
	struct Srep;	//representation
	Srep *rep;
public:
	String();				//x=""
	String(const C*);	//x="abc"
	String(const String&);	//x=other_string

	String& operator=(const C* s){
		if (rep->n == 1)					//recycle Srep
			rep->assign(strlen(s), s);
		else{								//use new Srep
			rep->n--;
			rep = new Srep(strlen(s), s);
		}
		return *this;
	}
	String& operator+=(const String& x){
		//copy assignment
		x.rep->n++;
		if (--rep->n == 0)
			delete rep;
		rep = x.rep;
		return *this;
	};
	String& operator+=(const C*);

	friend ostream& operator<<(ostream& b, const String& a){
		cerr << a.rep->s;
		return b;
	};
	friend istream& operator>>(istream& b, String& a){
		string q;
		b >> q;
		const C* p = q.c_str();
		a.rep->assign(strlen(p), p);
		return b;
	}
	friend bool operator==(const String& x, const C* s){
		return strcmp(x.rep->s, s) == 0;
	}

	friend bool operator==(const String& x, const String& y){
		return strcmp(x.rep->s, y.rep->s) == 0;
	}

	friend bool operator!=(const String& x, const C* s){
		return strcmp(x.rep->s, s) != 0;
	}

	friend bool operator!=(const String& x, const String& y){
		return strcmp(x.rep->s, y.rep->s) != 0;
	}

	~String();

	class Cref{			//reference to char
		friend class String;
		String& s;
		int i;
		Cref(String& ss, int ii) :s(ss), i(ii){}
	public:
		operator C()const{ return s.read(i); }		//yield value
		void operator=(C c){ s.write(i, c); }		//change value
	};

	class Range{};	//for exceptions

	void check(int i)const { if (i < 0 || rep->sz <= i)throw Range(); }

	C read(int i)const { return rep->s[i]; }
	void write(int i, C c){ rep = rep->get_own_copy(); rep->s[i] = c; }

	Cref operator[](int i) { check(i); return Cref(*this, i); }
	C operator[](int i) const { check(i); return rep->s[i]; }

	int size() const{ return rep->sz; }
};

template<class C>struct String<C>::Srep{
	C* s;	//pointer to elements
	int sz;		//number of characters
	int n;		//reference count

	Srep(int nsz, const C* p){
		n = 1;
		sz = nsz;
		s = new C[sz + 1];	//add space for terminator
		strcpy(s, p);
	}

	~Srep(){ delete[] s; }

	Srep* get_own_copy(){		//clone if necessary
		if (n == 1)
			return this;
		n--;
		return new Srep(sz, s);
	}

	void assign(int nsz, const C*p){
		if (sz != nsz){
			delete[] s;
			sz = nsz;
			s = new C[sz + 1];
		}
		strcpy(s, p);
	}
private:						//prevent copying:
	Srep(const Srep&);
	Srep& operator=(const Srep&);
};

/*
int hash(const class C&s){
int h = s.read(0);
const int max = s.size();
for (int i = 1; i < max; i++)h ^= s.read(i) >> 1;	//unchecked access to s
return h;
}
*/


template<class C> String<C>& operator+(const String<C>&, const String<C>&);
template<class C> String<C>& operator+(const String<C>&, const C*);

template<class C> String<C> f(String<C> a, String<C> b){
	a[2] = 'x';
	char c = b[3];
	cout << "in f:" << a << ' ' << b << ' ' << c << endl;
	return b;
}

template<class C> String<C>::String(){				//the empty string is the default value
	rep = new Srep(0, "");
}

template<class C> String<C>::String(const String& x){//copy constructor
	x.rep->n++;
	rep = x.rep;	//share representation
}

//Pseudoo-copy operations taking const char* arguments are provided to allow string literals:
template<class C> String<C>::String(const C* s){
	rep = new Srep(strlen(s), s);
}

template<class C> String<C>::~String(){
	if (--rep->n == 0)
		delete rep;
}

int main()
{
	String<char> buf;
	cin >> buf;
	cout << buf << endl;
	map<String<char>, int> m;
	// m[buf]++;//error C2784 and error C2676
	//write out result
	system("pause");
	return 0;
}