#include <iostream>
using namespace std;
#pragma warning(disable:4996)
#include <map>
#include <set>
#include <list>
#define RIVAL_NUM 4	// ��������

// �������
struct sPlayerData
{
	uint32_t	Rank;		// ����
	uint64_t	PlayerID;	// ���id
};

// ѡ����ֵĲ���
enum eSelectStrategy
{
	eSelectStrategy_LeftRight = 1,	// �������(�����������ǰ��,������"ǿ��")
	eSelectStrategy_RightLeft = 2,	// ���Һ���(�ұ������������,������"����")
};

class YingYuanFight
{
public:
	// ƥ���㷨(ˢ��Ҳ���������)
	void Match(int64_t RoleID)
	{
		sPlayerData* pData = GetDataByRole(RoleID);
		if (!pData)
			return;

		uint32_t Rank = pData->Rank;
		// �ų����Ľ�ɫ
		std::set<uint64_t> ExcludeRoleList;	//�ų����Ľ�ɫ�б�
		ExcludeRoleList.insert(RoleID);

		// �ҵ��Ķ����б�
		std::list<uint64_t> RivalRoleList;

		// �Ҳ�����,�򲻼�������,�����ҵ��Ĳ�������.
		std::list<uint32_t> list_input;	// 4���������
		if (list_input.size() != RIVAL_NUM)
			return;

		eSelectStrategy Strategy = eSelectStrategy_LeftRight;
		for (auto PercentValue : list_input)
		{
			// ������� TODO
			uint32_t BeginPoint = (uint32_t)(Rank * (PercentValue % 100 / 100.f));
			uint64_t OutRoleID = 0;
			if (SelectOne(BeginPoint, Strategy, ExcludeRoleList, OutRoleID))
				RivalRoleList.push_back(OutRoleID); else
				break;
		}
		// ���ز��ҵ������н�ɫ
	}

	// ѡ��һ������
	bool SelectOne(uint32_t BeginPoint, eSelectStrategy Strategy, std::set<uint64_t> ExcludeRoleList, uint64_t& OutRoleID)
	{
		// Ϊfalse��ʾû���ҵ�
		bool bFind = true;

		// ����������������
		if (Strategy == eSelectStrategy_LeftRight)
			if (!(bFind = FindLeft(BeginPoint, ExcludeRoleList, OutRoleID)))
				bFind = FindRight(BeginPoint, ExcludeRoleList, OutRoleID);
		else if (Strategy == eSelectStrategy_RightLeft)	// ���Һ���
			if (!(bFind = FindRight(BeginPoint, ExcludeRoleList, OutRoleID)))
				bFind = FindLeft(BeginPoint, ExcludeRoleList, OutRoleID);
		return bFind;
	}

	// ���Ҳ���
	bool FindRight(uint32_t BeginPoint, std::set<uint64_t> ExcludeRoleList, uint64_t& OutRoleID)
	{
		auto middle_iter = m_TotalRank.lower_bound(BeginPoint);
		for (auto iter = middle_iter; iter != m_TotalRank.end(); ++iter)
		{
			if (!iter->second)
				continue;
			if (ExcludeRoleList.find(iter->second->PlayerID) != ExcludeRoleList.end())
				continue;

			OutRoleID = iter->second->PlayerID;
			ExcludeRoleList.insert(OutRoleID);
			return true;
		}
		return false;
	}

	// �������
	bool FindLeft(uint32_t BeginPoint, std::set<uint64_t> ExcludeRoleList, uint64_t& OutRoleID)
	{
		auto middle_iter = m_TotalRank.lower_bound(BeginPoint);
		RankList_Type::reverse_iterator reverse_iter(middle_iter);
		for (auto iter = reverse_iter; iter != m_TotalRank.rbegin(); ++iter)
		{
			if (!iter->second)
				continue;
			if (ExcludeRoleList.find(iter->second->PlayerID) != ExcludeRoleList.end())
				continue;

			OutRoleID = iter->second->PlayerID;
			ExcludeRoleList.insert(OutRoleID);
			return true;
		}
		return false;
	}

private:
	// ͨ����ɫid��ȡ��������
	sPlayerData* GetDataByRole(uint64_t RoleID)
	{
		if (m_RankList_Role.find(RoleID) != m_RankList_Role.end())
			return m_RankList_Role[RoleID];
		return nullptr;
	}

	// �������������������
	sPlayerData* GetDataByRank(uint32_t Rank)
	{
		if (m_TotalRank.find(Rank) != m_TotalRank.end())
			return m_TotalRank[Rank];
		return NULL;
	}

private:
	typedef std::map<uint32_t, sPlayerData*> RankList_Type;			// ������Ϊkey
	typedef std::map<uint64_t, sPlayerData*> RankList_Role_Type;	// �Խ�ɫidΪkey
	
	RankList_Role_Type					m_RankList_Role;// �Խ�ɫid
	RankList_Type						m_TotalRank;	// �ܰ�
	std::map<uint8_t, RankList_Type>	m_CampRank;		// ��Ӫ��

};

int main()
{

	return 0;
}