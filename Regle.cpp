#include "Regle.h"
#include <map>

using namespace std;

Regle::Regle()
{}

Regle::Regle(std::vector< Predicat* > _conditions, vector< std::vector< unsigned int > > _var_cond, vector< std::vector< unsigned int > > _var_concl, std::vector< Predicat* > _conclusions)
:conditions(_conditions), var_cond(_var_cond), var_concl(_var_concl), conclusions(_conclusions)
{}

vector< Predicat* > Regle::get_conditions()
{
  return conditions;
}

vector< std::vector< unsigned int > > Regle::get_var_cond()
{
  return var_cond;
}

vector< std::vector< unsigned int > > Regle::get_var_concl()
{
  return var_concl;
}

void Regle::verifyCA(std::map< unsigned int, Variable* > m, unsigned int n, Predicat* p, std::vector< Predicat* >& preds)
{
  map<unsigned int, Variable*> mprime;
  bool instance=true;
  for (unsigned int i=0; i<p->get_variables().size(); ++i)
  {
    mprime = m;
    instance = true;
    for (unsigned int j=0; j<p->get_arite(); ++j)
    {
      if (mprime[var_cond[n][j]] == nullptr)
      {
	(mprime.at(var_cond[n][j]))=(p->get_variables()[i][j]);
      }
      else
      {
	if(mprime[var_cond[n][j]] != p->get_variables()[i][j])
	{
	  instance = false;
	}
      }
    }
    if (instance == true)
    {
      if (n == conditions.size()-1)
      {
	vector<Variable*> var;
	for (unsigned int k=0; k<conclusions.size(); ++k)
	{
	  for (unsigned int j=0; j<var_concl[k].size(); ++j)
	  {
	    var.push_back(mprime[var_concl[k][j]]);
	  }
	  if (!(conclusions[k]->verify(var)))
	  {
	    //cout << "ajouté" << endl;
	    conclusions[k]->add_variables(var);
	    preds.push_back(conclusions[k]);
	  }
	}
      }
      else
      {
	verifyCA(mprime,n+1,conditions[n+1],preds);
      }
    }
  }
}

bool Regle::exist_concl(Predicat* p)
{
  for(unsigned int i=0; i<conclusions.size(); ++i)
    if (conclusions[i] == p)
      return true;
  return false;
}

void Regle::print(std::ostream& out)
{
  string rc="\n";
  string si="Si ";
  string et="et si ";
  string donc="alors ";
  string etd="et alors ";
  conditions[0]->add_var(var_cond[0]);
  out << si << *conditions[0] << rc;
  for (unsigned int i=1; i<conditions.size(); ++i)
  {
    conditions[i]->add_var(var_cond[i]);
    out << et << *conditions[i] << rc;
  }
  conclusions[0]->add_var(var_concl[0]);
  out << donc << *conclusions[0] << rc;
  for (unsigned int i=1; i<conclusions.size(); ++i)
  {
    conclusions[i]->add_var(var_concl[i]);
    out << etd << *conditions[i] << rc;
  }
}

Regle* Regle::fabriqueRegle(std::vector< Predicat* > _conditions, vector< std::vector< unsigned int > > _var_cond, vector< std::vector< unsigned int > > _var_concl, std::vector< Predicat* > _conclusions)
{
  if((_var_cond.size() != _conditions.size()) || (_var_concl.size() != _conclusions.size()))
  {
      cout << "Mauvaise instanciation de règle : les vecteurs conditions et var_cond ou les vecteurs conclusions et var_concl n'ont pas la même taille entre eux" << endl;
      return nullptr;
  }
  else{
    for (unsigned int i=0; i<_conditions.size(); ++i)
      if (_conditions[i]->get_arite() != _var_cond[i].size())
      {
	cout << "Mauvaise taille de var_cond pour l'arité du prédicat à la posisiton " << i << endl;
	return nullptr;
      }
    for (unsigned int i=0; i<_conclusions.size(); ++i)
      if (_conclusions[i]->get_arite() != _var_concl[i].size())
      {
	cout << "Mauvaise taille de var_cond pour l'arité du prédicat à la posisiton " << i << endl;
	return nullptr;
      }
  }
  return new Regle(_conditions, _var_cond, _var_concl, _conclusions);
}
