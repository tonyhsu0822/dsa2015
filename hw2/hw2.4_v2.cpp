#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
using namespace std;
#define MAX_USER 24000000
#define URL_LENGTH 50

typedef struct{
	int click;
	int impression;
	char url[URL_LENGTH];
	int adid;
	int aderid;
	int depth;
	int position;
	int queryid;
	int keywordid;
	int titleid;
	int descriptionid;
} Data;

vector<Data> datavec[MAX_USER];

bool dataCompForClicked(Data a, Data b){
	if(a.adid != b.adid)
		return (a.adid < b.adid);
	else
		return (a.queryid <= b.queryid);
}

bool dataCompForImpressed(Data a, Data b){
	if(a.adid != b.adid)
		return (a.adid < b.adid);
	else if(a.aderid != b.aderid)
		return (a.aderid < b.aderid);
	else if(a.keywordid != b.keywordid)
		return (a.keywordid < b.keywordid);
	else if(a.titleid != b.titleid)
		return (a.titleid < b.titleid);
	else if(a.descriptionid != b.descriptionid)
		return (a.descriptionid < b.descriptionid);
	else
		return (strcmp(b.url, a.url) <= 0);
}

void printVec(int u){
	for(vector<Data>::iterator it = datavec[u].begin(); it != datavec[u].end(); it++){
		cout << "click: " << it->click << " impression: " << it->impression;
		cout << " adid: " << it->adid << " queryid: " << it->queryid;
		cout << " position: " << it->position << " depth: " << it->depth << endl;
	}
	return;
}

void get(int u, int a, int q, int p, int d){
	int totalclick = 0;
	int totalimpression = 0;
	
	//cout << "datavec[" << u << "].size = " << datavec[u].size() << endl;
	for(vector<Data>::iterator it = datavec[u].begin(); it != datavec[u].end(); it++){
		if(it->adid == a && it->queryid == q && it->position == p && it->depth == d){
			totalclick += it->click;
			totalimpression += it->impression;
			//cout << "find one data\n";
		}
	}
	cout << totalclick << " " << totalimpression << endl;
	return;
}

void clicked(int u){
	int tmpadid = -1;
	int tmpqueryid = -1;
	
	//cout << "datavec[" << u << "].size = " << datavec[u].size() << endl;
	sort(datavec[u].begin(), datavec[u].end(), dataCompForClicked);
	for(vector<Data>::iterator it = datavec[u].begin(); it != datavec[u].end(); it++){
		if(it->click > 0){
			if(it->adid != tmpadid || it->queryid != tmpqueryid){
				cout << it->adid << " " << it->queryid << endl;
				tmpadid = it->adid;
				tmpqueryid = it->queryid;
			}
		}
	}
	return;
}

void impressed(int u1, int u2){
	unsigned int i = 0;
	unsigned int j = 0;
	//int tmpadid = -1;
	vector<Data> tmpvec;
	vector<int> sameadid;
	vector<int>::iterator it;
	vector<Data>::iterator it2;
	vector<Data>::iterator it3;
	
	sort(datavec[u1].begin(), datavec[u1].end(), dataCompForImpressed);
	sort(datavec[u2].begin(), datavec[u2].end(), dataCompForImpressed);
	
	//find the same adid
	while(i < datavec[u1].size() && j < datavec[u2].size()){
		if(datavec[u1][i].adid < datavec[u2][j].adid)
			i++;
		else if(datavec[u1][i].adid > datavec[u2][j].adid)
			j++;
		else{
			/*
			if(datavec[u1].adid != tmpadid){
				cout << datavec[u1].adid << endl;
				printf("\t%s %d %d %d %d\n", datavec[u1].url, datavec[u1].aderid, datavec[u1].keywordid, 
											 datavec[u1].titleid, datavec[u1].descriptionid);
				if(strcmp(datavec[u1].url, datavec[u2].url) != 0 ||
				   datavec[u1].aderid != datavec[u2].aderid ||
				   datavec[u1].keywordid != datavec[u2].keywordid ||
				   datavec[u1].titleid != datavec[u2].titleid ||
				   datavec[u1].descriptionid != datavec[u2].descriptionid)
				{
					printf("\t%s %d %d %d %d\n", datavec[u2].url, datavec[u2].aderid,
												 datavec[u2].keywordid, datavec[u2].titleid,
												 datavec[u2].descriptionid);
				}
			}
			else if(){
			}
			tmpadid = datavec[u1].adid;
			
			tmpvec.push_back(datavec[u1][i]);
			tmpvec.push_back(datavec[u2][j]);
			*/
			it = find(sameadid.begin(), sameadid.end(), datavec[u1][i].adid);
			if(it == sameadid.end())
				sameadid.push_back(datavec[u1][i].adid);
			i++; j++;
		}
	}
	
	//output ad properties
	for(it = sameadid.begin(); it != sameadid.end(); it++){
		cout << *it << endl;
		//of u1
		//move to the element which has the adid in the sameadid
		for(it2 = datavec[u1].begin(); it2 != datavec[u1].end() && it2->adid != *it; it2++);
		//output unique properties
		for(; it2->adid == *it; it2++){
			for(it3 = tmpvec.begin(); it3 != tmpvec.end(); it3++){
				if(it2->aderid == it3->aderid && it2->keywordid == it3->keywordid &&
				   it2->titleid == it3->titleid && it2->descriptionid == it3->descriptionid &&
				   strcmp(it2->url, it3->url) == 0) break;
			}
			if(it3 == tmpvec.end()){
				printf("\t%s %d %d %d %d\n", it2->url, it2->aderid, it2->keywordid, 
											 it2->titleid, it2->descriptionid);
				tmpvec.push_back(*it2);
			}
		}
		//of u2
		for(it2 = datavec[u2].begin(); it2 != datavec[u2].end() && it2->adid != *it; it2++);
		for(; it2->adid == *it; it2++){
			for(it3 = tmpvec.begin(); it3 != tmpvec.end(); it3++){
				if(it2->aderid == it3->aderid && it2->keywordid == it3->keywordid &&
				   it2->titleid == it3->titleid && it2->descriptionid == it3->descriptionid &&
				   strcmp(it2->url, it3->url) == 0) break;
			}
			if(it3 == tmpvec.end()){
				printf("\t%s %d %d %d %d\n", it2->url, it2->aderid, it2->keywordid, 
											 it2->titleid, it2->descriptionid);
				tmpvec.push_back(*it2);
			}
		}
	}
	
	return;
}	

void profit(int a, double theta){
	double tmpsumclick;
	double tmpsumimpression;
	double rate;
	
	for(int i = 0; i < MAX_USER; i++){
		tmpsumclick = 0;
		tmpsumimpression = 0;
		for(vector<Data>::iterator it = datavec[i].begin(); it != datavec[i].end(); it++){
			if(it->adid == a){
				tmpsumclick += it->click;
				tmpsumimpression += it->impression;
			}
		}
		if(tmpsumimpression == 0) continue;
		rate = tmpsumclick / tmpsumimpression;
		if(rate >= theta)
			cout << i << endl;
	}
	return;
}

int main(int argc, char *argv[]){
	Data tmpdata;
	int tmpuserid;
	FILE *fp;
	string cmd;
	int u1, u2, a, q, p, d;
	double theta;
	
	if((fp = fopen(argv[1], "r")) == NULL){
		cout << "fopen fail\n";
		return 0;
	}
	
	while(fscanf(fp, "%d\t%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
					&tmpdata.click, &tmpdata.impression, tmpdata.url, &tmpdata.adid, &tmpdata.aderid, 
					&tmpdata.depth, &tmpdata.position, &tmpdata.queryid, &tmpdata.keywordid,
					&tmpdata.titleid, &tmpdata.descriptionid, &tmpuserid) != EOF)
	{
		if(datavec[tmpuserid].capacity() == 0)
			datavec[tmpuserid].reserve(16);
		datavec[tmpuserid].push_back(tmpdata);
	}
	
	//test part
	while(1){
		cin >> cmd;
		if(cmd == "get"){
			cin >> u1 >> a >> q >> p >> d;
			cout << "********************" << endl;
			get(u1, a, q, p, d);
			cout << "********************" << endl;
		}
		else if(cmd == "clicked"){
			cin >> u1;
			cout << "********************" << endl;
			clicked(u1);
			cout << "********************" << endl;
		}
		else if(cmd == "impressed"){
			cin >> u1 >> u2;
			cout << "********************" << endl;
			impressed(u1, u2);
			cout << "********************" << endl;
		}
		else if(cmd == "profit"){
			cin >> a >> theta;
			cout << "********************" << endl;
			profit(a, theta);
			cout << "********************" << endl;
		}
		else if(cmd == "quit")
				break;
	}
	
	fclose(fp);
	return 0;
}







