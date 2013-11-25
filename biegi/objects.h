static const int DrawType = 1;

struct Vector3
{
	float x;
	float y;
	float z;
};

class Stadium
{
public:
	Stadium(float length, int lanes, float lanesize);
	~Stadium();
	void draw();
	float length;
	int lanes;
	float lanesize;
	float rin;
};

class Tree
{
public:
	Tree(float size, float x=0, float y=0, float z=0);
	~Tree();
	void draw();
private:
	void drawTree(float size);
	float size;
	Vector3 *branches;
	int bcount;
	Vector3 position;
	Vector3 leafcolor;
	Vector3 basecolor;
	static const int MaxBranches = 10;
};

