#pragma once

class Pu
{
public:
	explicit Pu(int id): m_id(id){}
	bool operator < (const Pu& rhs){ return true; }
	//Accessors
	int GetID() const { return m_id; }

private:
	int m_id;
};
