#include <vector>
#include <string>
vector<string> yy;
#include <iostream>
#include <iomanip>
#include <mysql++>

int main() {
  Connection con(use_exceptions);
	try {
		con.real_connect ("test","localhost","root","",3306,(int)0,60,NULL);
					
		Query query = con.query();
		query.exec("DROP TABLE IF EXISTS jedan");
		query.exec("DROP TABLE IF EXISTS dva");		
		query.exec("DROP TABLE IF EXISTS tri");
		query.exec("CREATE TABLE jedan (id1 int(11) auto_increment primary key, pisanije char(12), vreme timestamp)");
		query.exec("CREATE TABLE dva   (id2 int(11), pisanije char(12), vreme timestamp, INDEX(id2))");
		query.exec("CREATE TABLE tri   (id3 int(11), pisanije char(12), vreme timestamp, INDEX(id3))");
		for (unsigned int i=0; i<1000;i++) {
			char c = 'a' + i % 25; char xx[11]; memset(xx,c,10); xx[10]='\0'; 
			char buff [100];
			sprintf (buff,"INSERT INTO jedan (pisanije) VALUES('%s')",xx);
			query.exec(buff);
			int numer=i+1;
			for (unsigned j=0; j < 10; j++) {
				sprintf (buff,"INSERT INTO dva (id2,pisanije) VALUES(%d,\'%s\')",numer,xx);
				query.exec(buff);
				if ((j % 2)) {
					sprintf (buff,"INSERT INTO tri (id3,pisanije) VALUES(%d,\'%s\')",numer,xx);
					query.exec(buff);
				}
			}
		}

  } catch (BadQuery er) {
    cerr << "Error: " << er.error << " " << con.errnum() << endl;
    return -1;
  }
}

