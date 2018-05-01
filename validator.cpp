#include "Tag.hpp"
#include <stack>
#include <queue>
#include<algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <sstream>



bool html_is_valid (const std::string& documen) {
// first, if I find any opening tag, i'll push corresponding closing tag to stack. As soon as I find any closing tag,
// i would try to pop the stack and the popped values must be equal to the closing tag found; as whatever closes, should
// have opened right before it.


//    copy of the input string.
    std::string document= documen;

//DONE make multiple consecutive white spaces into singular, aka parsing
    //(\n and \t) also taken care of
    for(auto i=0; i<(document.length());i++){
        if((document[i]==' '||document[i]=='\t'||document[i]=='\n')&&(document[i+1]==' '||document[i+1]=='\n'||document[i+1]=='\t')){
            document.erase(document.begin()+i);
            i=0;
        }
    }

    std::stack<std::string> data;

    int html =0;
    int head =0;
    int title=0;
    int body=0;
    int doctype=0;


    for(int i=0; i<document.length();i++){
//there should be these five tags for sure, in an html file

        if(document[i]=='<' && document[i+1]=='!' && (document[i+2]=='d' ||document[i+2]=='D')   && (document[i+3]=='o'||document[i+3]=='O')  && (document[i+4]=='c' ||document[i+4]=='C')
           && (document[i+5]=='t'||document[i+5]=='T')  && (document[i+6]=='y'||document[i+6]=='Y')  && (document[i+7]=='p'||document[i+7]=='P' ) && (document[i+8]=='e'||document[i+8]=='E' ) ) {
            doctype++;  //everything cool, doctype opening present
        }

        if(document[i]=='<' && (document[i+1]=='h'||document[i+1]=='H' ) && (document[i+2]=='t' ||document[i+2]=='T')
           && (document[i+3]=='m'||document[i+3]=='M' ) && (document[i+4]=='l' ||document[i+4]=='L' )) {
            html++;  //everything cool, html opening present
        }
        if(document[i]=='<' && (document[i+1]=='h'||document[i+1]=='H' ) && (document[i+2]=='e'||document[i+2]=='E')
           && (document[i+3]=='a'||document[i+3]=='A' ) && (document[i+4]=='d'||document[i+4]=='D' ) ) {
            head++;   //everything cool, head opening present
        }
        if(document[i]=='<' && (document[i+1]=='t'||document[i+1]=='T' ) && (document[i+2]=='i'||document[i+2]=='I')
           && (document[i+3]=='t'||document[i+3]=='T' ) && (document[i+4]=='l'||document[i+4]=='L' ) && (document[i+5]=='e'||document[i+5]=='E' )) {
            title++; //everything cool, title opening present
        }
        if(document[i]=='<' && (document[i+1]=='b'||document[i+1]=='B' ) && (document[i+2]=='o'||document[i+2]=='O')
           && (document[i+3]=='d'||document[i+3]=='D')  && (document[i+4]=='y'||document[i+4]=='Y' )){
            body++; //everything cool, body opening present
        }
    }
    if(doctype!=1 || html!=1 || body!=1 || head!=1 || title!=1){
        return false;
    }


//test:: after every'>', the next element should be '<'
    //sequencing of tags and their tight bound not checked
    // after html, it is always head
    //after head, always body
    //after body always html closing tag
    //head has only title , and no other char or whitespace
    //everything should be tightly bound, no extra whitespace other than rules.


    //white space invalidity only within tags, so in parsing, we dont touch whitespaces within tags..


////push pop stack checking
    for(int i=0; i<document.length(); i++){

        //push, if opening tag found
        if(document[i]=='<' && document[i+1]=='h' && document[i+2]=='t' && document[i+3]=='m' && document[i+4]=='l' ) {
            if(document[i+9]=='"' && (document[i+10]==' '||document[i+10]=='\t'||document[i+10]=='\n' || document[i+10]=='"') ){ //check if the value of id is a blank
                return false;
            }
            data.push("</html>");

            for(int j=i+1; document[j]!='<';j++){  ////head should be there after html
                if(document[j+2]=='<' && document[j+3]!='h'){
                    return false;
                }
            }
        }
        if(document[i]=='<' && document[i+1]=='h' && document[i+2]=='e' && document[i+3]=='a' && document[i+4]=='d' ) {
            if(document[i+9]=='"' && (document[i+10]==' '||document[i+10]=='\t'||document[i+10]=='\n' || document[i+10]=='"')) { //check if the value of id is a blank
                return false;
            }
            if(data.size()==0){  //head is the first tag
                return false;
            }
            if(data.size()!=0){
                std::string & s = data.top();

                if(s!="</html>"){ ////////invalid-head-before-html
                    return false;
                }
            }

            for(int j=i+1; document[j]!='<';j++){  ////head has no title
                if(document[j+2]=='<' && document[j+3]=='/'){
                    return false;
                }
            }

            for(int j=i+1; document[j]!='<';j++){  ////element between head and title
                if(document[j+2]=='<' && document[j+3]!='t'){
                    return false;
                }
            }

            data.push("</head>");
        }
        if(document[i]=='<' && document[i+1]=='t' && document[i+2]=='i' && document[i+3]=='t' && document[i+4]=='l' && document[i+5]=='e') {
            if(document[i+10]=='"' && (document[i+11]==' '||document[i+11]=='\t'||document[i+11]=='\n' || document[i+11]=='"') ){ //check if the value of id is a blank or empty
                return false;
            }
            if(data.size()!=0){
                std::string & s = data.top();

                if(s!="</head>"){ ////////invalid-title-before-head
                    return false;
                }
            }
            data.push("</title>");
            for(int j=i; document[j]!='>';j++){
                if (document[j+2]=='<'|| document[j+3]=='<'){
                    return false; // title has no content
                }
            }

        }
        if(document[i]=='<' && document[i+1]=='b' && document[i+2]=='o' && document[i+3]=='d' && document[i+4]=='y' ){
            if(document[i+9]=='"' && (document[i+10]==' '||document[i+10]=='\t'||document[i+10]=='\n' || document[i+10]=='"') ){ //check if the value of id is a blank
                return false;
            }
            if(data.size()!=0){
                std::string & s = data.top();
                if(s!="</html>"){ ////////head has ended before body began
                    return false;
                }
            }
            data.push("</body>");
        }
        if(document[i]=='<' && document[i+1]=='d' && document[i+2]=='i' && document[i+3]=='v') {
            if(document[i+8]=='"' && (document[i+9]==' '||document[i+9]=='\t'||document[i+9]=='\n' || document[i+9]=='"') ){ //check if the value of id is a blank
                return false;
            }
            data.push("</div>");
        }
        if(document[i]=='<' && document[i+1]=='s' && document[i+2]=='p' && document[i+3]=='a' && document[i+4]=='n') {
            if(document[i+9]=='"' && (document[i+10]==' '||document[i+10]=='\t'||document[i+10]=='\n' || document[i+10]=='"')) { //check if the value of id is a blank
                return false;
            }

            for(int j=i+1; document[j]!='<';j++){  ////invalid tag in span
                if(document[j+2]=='<' && document[j+3]!='/'){
                    return false;
                }
            }
            data.push("</span>");
        }
        if(document[i]=='<' && document[i+1]=='p') {
            if(document[i+6]=='"' && (document[i+7]==' '||document[i+7]=='\t'||document[i+7]=='\n' || document[i+7]=='"')){ //check if the value of id is a blank
                return false;
            }
            data.push("</p>");
        }

        //test for no space in between 2 words in any id
        if(document[i]=='i' && document[i+1]=='d' && document[i+2]=='='){
            for(int j=i+4; document[j]!='"';j++){
                if(document[j]==' '||document[j]=='\t'||document[j]=='\n'){
                    return false;
                }
            }
        }

        //validity check for self closing tag <br> or <br/>
        // <br >  and <br /> are valid
        //<br / >  invalid
        if(document[i]=='/' && (document[i+1]==' '||document[i+1]=='\t'||document[i+1]=='\n') && document[i+2]=='>') {
            return false;
        }

        //invalid: cant have space between < and tagname in opening tag
        // between < and / and tagname in closing tag
        // inside an id of a tag

        //validity check for no space between opening angle and tagname for opening tag
        if(document[i]=='<' && (document[i+1]==' '||document[i+1]=='\t'||document[i+1]=='\n')) {
            return false;
        }
        //validity check for no space between opening angle and slash and tagname for closing tag
        if(document[i]=='<' && document[i+1]=='/' && (document[i+2]==' ' ||document[i+2]=='\t'||document[i+2]=='\n')) {
            return false;
        }
        //validity check for no space between opening angle and ! and tagname for <!doctype html>
        if(document[i]=='<' && document[i+1]=='!' && (document[i+2]==' '||document[i+2]=='\t'||document[i+2]=='\n')) {
            return false;
        }


        //pop if closing tag found, compare, retval set f if not equal;
        //removed closing angle brackets as there may be id present

        if(document[i]=='<'&& document[i+1]=='/'  && document[i+2]=='h' && document[i+3]=='t' && document[i+4]=='m' && document[i+5]=='l') {
            if(document[i+7]=='i' ){
                return false; // if id present in closing tag
            }
            if(!data.empty() && data.top() =="</html>") {
                data.pop();
            }else{
                return false;
            }
        }
        if(document[i]=='<'&& document[i+1]=='/'  && document[i+2]=='h' && document[i+3]=='e' && document[i+4]=='a' && document[i+5]=='d') {
            if(document[i+7]=='i' ){
                return false; // if id present in closing tag
            }
            if(!data.empty() && data.top() =="</head>") {
                data.pop();
            }else{
                return false;
            }
        }
        if(document[i]=='<'&& document[i+1]=='/'  && document[i+2]=='t' && document[i+3]=='i' && document[i+4]=='t' && document[i+5]=='l' && document[i+6]=='e') {
            if(document[i+8]=='i' ){
                return false; // if id present in closing tag
            }

            for(int j=i+1; document[j]!='<';j++){  ////element between title and closing of head
                if(document[j+2]=='<' && (document[j+3]!='/' || document[j+4]!='h')){
                    return false;
                }
            }

            if(!data.empty() && data.top() =="</title>") {
                data.pop();
            }else{
                return false;
            }
        }
        if(document[i]=='<'&& document[i+1]=='/'  && document[i+2]=='b' && document[i+3]=='o' && document[i+4]=='d' && document[i+5]=='y') {
            if(document[i+7]=='i' ){
                return false; // if id present in closing tag
            }
            if(!data.empty() && data.top() =="</body>") {
                data.pop();
            }else{
                return false;
            }
        }
        if(document[i]=='<'&& document[i+1]=='/'  && document[i+2]=='d' && document[i+3]=='i' && document[i+4]=='v')  {
            if(document[i+6]=='i' ){
                return false; // if id present in closing tag
            }
            if(!data.empty() && data.top() =="</div>") {
                data.pop();
            }else{
                return false;
            }
        }
        if(document[i]=='<'&& document[i+1]=='/'  && document[i+2]=='s' && document[i+3]=='p' && document[i+4]=='a' && document[i+5]=='n') {
            if(document[i+7]=='i' ){
                return false; // if id present in closing tag
            }
            if(!data.empty() && data.top() =="</span>") {
                data.pop();
            }else{
                return false;
            }
        }
        if(document[i]=='<'&& document[i+1]=='/'  && document[i+2]=='p') {
            if(document[i+4]=='i' ){
                return false; // if id present in closing tag
            }
            if(!data.empty() && data.top() =="</p>") {
                data.pop();
            }else{

                return false;
            }
        }
    }


    if(data.size()!=0){ // check for remainder html header, in case closing tag wasnt present for comparision
        return false;
    }

    return true;
}

//void clean_tree(Tag* root) {
//    if (root == nullptr) {
//        return;
//    }
//    std::queue<Tag*> nodesToCleanup;
//    nodesToCleanup.push(root);
//    while (!nodesToCleanup.empty()) {
//        Tag* element = nodesToCleanup.front();
//        nodesToCleanup.pop();
//        for (Tag* child : element->_children) {
//            nodesToCleanup.push(child);
//        }
//        delete element;
//    }
//}

Tag* generate_DOM_tree(const std::string& documen) {

    std::string document= documen;

//    for(int i=0; i<document.size();i++){
//        document[i]= tolower(document[i]);
//
//    }

    //DONE make multiple consecutive white spaces into singular ; (\n and \t) also taken care of
    for(auto i=0; i<document.length();i++){
        if((document[i]==' '||document[i]=='\t'||document[i]=='\n')&&(document[i+1]==' '||document[i+1]=='\n'||document[i+1]=='\t')){
            document.erase(document.begin()+i);
            i=0;
        }
        if(document[i]=='\n'){
            document.erase(document.begin()+i);
        }
        if(document[i]=='>' && (document[i+1]==' '||document[i+1]=='\n'||document[i+1]=='\t')){
            document.erase(document.begin()+i+1);
            i=0;
        }
        if(i-1>=0 && document[i]=='<' && (document[i-1]==' '||document[i-1]=='\n'||document[i-1]=='\t')){
            document.erase(document.begin()+i-1);
            i=0;
        }
        if(document[i]=='i' && document[i+1]=='d' && document[i+3]=='='){ ////removing whitespace in between id and ""
                    document.erase(document.begin()+i+2 );
//                for(int j=i ;document[j]!='>';j++){
//                if(document[j]==' ' || document[j]=='\n' || document[j]=='\t'){
//                    document.erase(document.begin()+j);
//
//                }
//            }
        }
        if(document[i]=='i'&& document[i+1]=='d' && document[i+2]=='='&& document[i+4]=='"'){
            document.erase(document.begin()+i+3 );
        }
        if(document[i]=='i'&& document[i+1]=='d'&& document[i+2]==' ' && document[i+3]=='=' && document[i+4]==' '&& document[i+5]=='"'){
            document.erase(document.begin()+i+2 );
            document.erase(document.begin()+i+4 );
        }
    }


    std::stack<Tag*> data;
    std::string htmlid="";

    ////getting id for the html or HTML
    for(int i=0; i<document.length();i++) {

            if(document[i]=='<' && (document[i+1]=='h'||document[i+1]=='H' ) && (document[i+2]=='t'||document[i+2]=='T')
               && (document[i+3]=='m'||document[i+3]=='M' ) && (document[i+4]=='l'||document[i+4]=='L' ) &&document[i+6]=='i'&& document[i+7]=='d'  ) {


//                int p=i+10;
                for(int j=i+10; document[j]!='"'; j++){
                    htmlid += document[j];
                }

        }
    }

    Tag* root = new Tag("html",Tag::TAGNAME::HTML,htmlid);
    data.push(root);
    Tag* pointer = data.top();


    for(int i=0; i<document.length();i++) {

        ////pop if closing tag found
        if(document[i]=='<'&& document[i+1]=='/' && data.size()>0){

            data.pop();
            if(data.size()>0){
                pointer= data.top();
            }else{
                pointer = root;
            }

        }

        ////push, if opening tag found

        if(document[i]=='<' && (document[i+1]=='h'||document[i+1]=='H' ) && (document[i+2]=='e'||document[i+2]=='E')
           && (document[i+3]=='a'||document[i+3]=='A' ) && (document[i+4]=='d'||document[i+4]=='D' ) ) {

                std::string id = "";

            if (document[i + 6] == 'i' && document[i + 7] == 'd' && document[i + 8] == '=') {
                for (int p = i + 10; document[p] != '"'; p++) {
                    id += document[p];
                }

            }
//            if(document[i+6]=='i') { //check the value of id
//                int start= 0, end =0;
//
//                for(int k=i; document[k+2]!='>'; k++){
//                    if(document[k]=='"' ){     start = k+1; }
//                }
//                for(int k=start; document[k]!='"';k++){
//                    if(document[k+1]=='"'){      end = k+1; }
//                }
//                for(int k=start; k<end; k++){    id+=document[k]; //here we dont include end as end points (")
//                }
//            }


            Tag *head = new Tag("head", Tag::TAGNAME::HEAD ,id);
            ((*pointer)._children).push_back(head);
            pointer = head;
            data.push(head);

            ///content shall be only there as child of:  body, title, div, p, span
        }

        if(document[i]=='<' && (document[i+1]=='t'||document[i+1]=='T' ) && (document[i+2]=='i'||document[i+2]=='I')
           && (document[i+3]=='t'||document[i+3]=='T' ) && (document[i+4]=='l'||document[i+4]=='L' ) && (document[i+5]=='e'||document[i+5]=='E' )) {

                std::string id = "";
            std::string content = "";
//            int z;
            if (document[i + 7] == 'i' && document[i + 8] == 'd' && document[i + 9] == '=') {
                for (int p = i + 11; document[p] != '"'; p++) {
                    id += document[p];
//                    z =p;
                }

//                if(document[z+2]=='>' ){  ////adding content for title with id
//                        for(int a=z+1; document[a]!='<';a++){
//                            content += document[a];
//                        }
//                    }

                for (int k = i; document[k] != '>'; k++) { ////adding content to title with id

                    if(document[k+2]=='>' ){
                        for(int a=k+3; document[a]!='<';a++){
                            content += document[a];
                        }
                    }
                }


            }
            if (document[i + 6] == '>')  { //access the start of content
                for (int k = i + 7; document[k] != '<'; k++) {
                    content += document[k];
                }
            }

            if (document[i + 7] == '>')  { //access the start of content
                for (int k = i + 8; document[k] != '<'; k++) {
                    content += document[k];
                }
            }
            Tag *title = new Tag("title",Tag::TAGNAME::TITLE, id);

            title->_content = content;  //content added for the title tag
            pointer->_children.push_back(title);
            pointer = title;
            data.push(title);
        }

        if(document[i]=='<' && (document[i+1]=='b'||document[i+1]=='B' ) && (document[i+2]=='o'||document[i+2]=='O')
           && (document[i+3]=='d'||document[i+3]=='D')  && (document[i+4]=='y'||document[i+4]=='Y' )){


                std::string id = "";
            if (document[i + 6] == 'i' && document[i + 7] == 'd' && document[i + 8] == '=') {
                for (int p = i + 10; document[p] != '"'; p++) {
                    id += document[p];
                }

            }
//            if(document[i+6]=='i') { //check the value of id
//                int start= 0, end =0;
//
//                for(int k=i; document[k+2]!='>'; k++){
//                    if(document[k]=='"' ){     start = k+1; }
//                }
//                for(int k=start; document[k]!='"';k++){
//                    if(document[k]=='"'){      end = k; }
//                }
//                for(int k=start; k!=end; k++){ id+=document[k];
//                }
//            }
            Tag *body = new Tag("body",Tag::TAGNAME::BODY , id);
            pointer->_children.push_back(body);
            pointer = body;
            data.push(body);
        }
        if (document[i] == '<' && document[i + 1] == 'd' && document[i + 2] == 'i' && document[i + 3] == 'v') {
            std::string id = "";

            ////

            if (document[i + 5] == 'i' && document[i + 6] == 'd' && document[i + 7] == '=') {
                for (int p = i + 9; document[p] != '"'; p++) {
                    id += document[p];
                }

            }
//            if(document[i+5]=='i'&& document[6]=='d') { //check the value of id
//                int start= 0, end =0;
//
//                for(int k=i; document[k+2]!='>'; k++){
//                    if(document[k]=='"' ){     start = k+1; }
//                }
//                for(int k=start; document[k]!='"';k++){
//                    if(document[k]=='"'){      end = k; }
//                }
//                for(int k=start; k!=end; k++){ id+=document[k];
//                }
//            }
            ////

            Tag *div = new Tag("div",Tag::TAGNAME::DIV , id);
            pointer->_children.push_back(div);
            pointer = div;
            data.push(div);
        }
        if (document[i] == '<' &&( document[i + 1] == 's' || document[i + 1] == 'S') && (document[i + 2] == 'p'||document[i + 2] == 'P' ) && (document[i + 3] == 'a' || document[i + 3] == 'A' ) &&
                (document[i + 4] == 'n'||document[i + 4] == 'N')) {



            std::string id = "";
            if (document[i + 6] == 'i' && document[i + 7] == 'd' && document[i + 8] == '=') {
                for (int p = i + 10; document[p] != '"'; p++) {
                    id += document[p];
                }

            }
//            if(document[i+6]=='i') { //check the value of id
//                int start= 0, end =0;
//
//                for(int k=i; document[k+2]!='>'; k++){
//                    if(document[k]=='"' ){     start = k+1; }
//                }
//                for(int k=start; document[k]!='"';k++){
//                    if(document[k]=='"'){      end = k; }
//                }
//                for(int k=start; k!=end; k++){ id+=document[k];
//                }
//            }

            Tag *span = new Tag("span",Tag::TAGNAME::SPAN , id);
            pointer->_children.push_back(span);
            pointer = span;
            data.push(span);
        }
        if (document[i] == '<' && document[i + 1] == 'p') {
            std::string id = "";
            if (document[i + 3] == 'i' && document[i + 4] == 'd' && document[i + 5] == '=') {
                for (int p = i + 7; document[p] != '"'; p++) {
                    id += document[p];
                }

            }
//            if(document[i+3]=='i') { //check the value of id
//                int start= 0, end =0;
//
//                for(int k=i; document[k+2]!='>'; k++){
//                    if(document[k]=='"' ){     start = k+1; }
//                }
//                for(int k=start; document[k]!='"';k++){
//                    if(document[k]=='"'){      end = k; }
//                }
//                for(int k=start; k!=end; k++){ id+=document[k];
//                }
//            }
            Tag *p = new Tag("p",Tag::TAGNAME::P , id);
            pointer->_children.push_back(p);
            pointer = p;
            data.push(p);
        }

        if (document[i] == '<' && document[i + 1] == 'b' && document[i+2]=='r') {

            std::string id = "";
            if (document[i + 4] == 'i' && document[i + 5] == 'd' && document[i + 6] == '=') {
                for (int p = i + 8; document[p] != '"'; p++) {
                    id += document[p];
                }
            }
            Tag *br = new Tag("br",Tag::TAGNAME::BR , id);
            pointer->_children.push_back(br);
//            pointer = br;
//            data.push(br);
        }

        /////////finding and adding content tag
        if ((i - 4) > 0 && (i + 2) < document.length() && document[i] == '>' && document[i + 1] != '<'

            && ((document[i-3]!='r' && document[i-4]!='u')  && document[i-1]!='e') ){   // solution for no content tag in title

//            if((i-28)>0 && (document[i-28]!='t' && document[i-27]!='i' && document[i-26]!='t')){

                std::string ctent = "";
//        int k;
                for (int p = i + 1; document[p] != '<'; p++) {
                    ctent += document[p];
//                    k=p+3;
                }

//            if((document[k+1]!='t'|| document[k+1]!='T')){
                Tag *content = new Tag("content",Tag::TAGNAME::CONTENT ,"");
                content->_content = ctent;
                pointer->_children.push_back(content);
//            }

//            }


        }


    }

    //while i dont find a closing tag, i'll keep on adding child to a particular index
    //if i find a closing tag, i'll point to the parent and then keep adding child.
    // while point to the parent, i point to the parent of closing tag that i found | done using stack pop

    //// white spaces parsed
    //// content: finding ,making new tag, and putting into data

    //// for any tag other that "content & title", this should be left empty
    //// content tag cannot have an empty string "" in its content
// after correct dom tree, try to test methods below.
    ////pop functionality to find particular parent is remaining



//    clean_tree(root);

    return root;
}


void determine_visible_objects(Tag* const root) {

    if(root->_name== "title" || root->_name=="content"){
        root->_displayed=true;
    }

    for(Tag * child : root->_children){
        determine_visible_objects(child);
    }

    for(Tag * child : root->_children){
        bool t=child->_displayed;

        if(t){
            root->_displayed=true;break;
        }
    }

//    if (root == nullptr) {
//        return;
//    }
//    std::queue<Tag*> nodes;
//    nodes.push(root);
//    while (!nodes.empty()) {
//        Tag* element = nodes.front();
//        if(element->_name == "content"){
//            if(element->_displayed==false){
//                element->_displayed=true;
//            }
//        }
//        for(int i=0;i<(element->_children.size());i++){
//            if(element->_children[i]->_displayed==true){
//                element->_displayed=true;
//                break;
//            }
//        }
//        nodes.pop();
//        for (Tag* child : element->_children) {
//            nodes.push(child);
//        }
//    }
    return;
}

std::string print_visible(Tag * root, int a){

    std::string output = "";

    bool t = root->_displayed;
    if(t){
        if(root->_name!="html"){
            output+='\n';
            for(int i=0;i<a ;i++){
                output+=' ';
            }
        }

        output+=root->_name;

        if(root->_content!=""){
            output+='\n';
            for(int i=0;i<a+2 ;i++){
                output+=' ';
            }
            output+=root->_content;
        }
    }

    for(Tag* child : root->_children){
        output += print_visible(child, a+2);
    }

    return output;
}

std::string print_visible_elements(Tag* const root) {
    return print_visible(root, 0);

}

Tag* getElementByID(Tag* const root, const std::string& id) {
    if (id == "") {
        return nullptr;
    }
    std::queue<Tag*> nodes;
    nodes.push(root);
    while(!nodes.empty()){
        Tag* element = nodes.front();

        if(element->_id==id){
            return element;
        }
        nodes.pop();
        for(Tag* child: element->_children){
            nodes.push(child);
        }

    }

    return nullptr;
}
