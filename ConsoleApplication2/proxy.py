"""header.length = sizeof(INT64) + ...;"""




queue = []
data_type = []


cpp_parsing_start = False
string_parse = ''
    
f_w_header = open("./Proxy.h","w")
f_w_header.write("#pragma once\n")
f_w_header.write("#include \"ProxyCaller.h\"\n\n")
f_w_header.write("class Proxy\n")              
f_w_header.write("{\n\n")
f_w_header.write("private:\n")
f_w_header.write("\tProxy();\n")
f_w_header.write("\t~Proxy();\n\n")
f_w_header.write("\tProxyCaller* Obj;\n\n")
f_w_header.write("public:\n\n")
f_w_header.write("\tstatic inline Proxy* GetInstance()\n")
f_w_header.write("\t{\n")
f_w_header.write("\t\tstatic Proxy proxy;\n")
f_w_header.write("\t\treturn &proxy;\n")
f_w_header.write("\t}\n")
f_w_header.write("\n\n\tvoid Set_Server(ProxyCaller* server)\n")
f_w_header.write("\t{\n")
f_w_header.write("\t\tthis->Obj = server;\n")
f_w_header.write("\t}\n\n")


f_w_cpp = open("./Proxy.cpp","w")
f_w_cpp.write("#include \"Proxy.h\"\n\n")
f_w_cpp.write("Proxy::~Proxy()\n")
f_w_cpp.write("{\n\n")
f_w_cpp.write("}\n\n")
f_w_cpp.write("Proxy::Proxy()\n")
f_w_cpp.write("{\n\n")
f_w_cpp.write("}\n\n")


f_r = open("./Proxy.txt","r")

def cpp_parsing(line):
    
    global cpp_parsing_start
    global string_parse
    
    s_line = line.split(" ")
    
    if cpp_parsing_start == False:
        string_parse=''
        s_line[1] = "Proxy::"+s_line[1]
        cpp_parsing_start = True
        
    
    string_parse += ' '.join(s_line)

    s_line = [v for v in s_line if v]

    if('\n' in s_line[-1]):
      
        cpp_parsing_start = False
        for i in range(-2,0):
            if(')' in s_line[i]):            
                last_sentence_add = s_line[i][:s_line[i].index(')')]
                if last_sentence_add != '':
                    queue.append(s_line[i][:s_line[i].index(')')])
                 
                else :
                    idx = i-1
                    while s_line[idx]=='':
                        idx=idx-1
                    queue.append(s_line[idx])
              
                break        
    elif s_line[-1] == '' :
        idx = -2
        while s_line[idx]=='':
            idx=idx-1
            
        queue.append(s_line[idx])
  
        string_parse += ',' 
        
        
        
    else:
        queue.append(s_line[-1])
        string_parse += ','
     


    string_parse = string_parse.replace(";","")

    txt=s_line[0]
    for i in range(1,len(s_line)-1):
        if(')' in s_line[i] or s_line[i] in queue): 
            
            break
        txt+=" "+s_line[i]  
    data_type.append(txt)
    



while True:
    line = f_r.readline()
    f_w_header.write("\n")
    if not line: break
    f_w_header.write("\t")
    f_w_header.write(line) 
    
   
    split_by_semicolon = line.split(",")
    if(split_by_semicolon[-1][-1]!='\n'):
        split_by_semicolon[-1] += '\n'
 
    for text in split_by_semicolon:
        cpp_parsing(text)
     
    f_w_cpp.write(string_parse)
    f_w_cpp.write("{\n")
    f_w_cpp.write("\tCPacket* pack = Obj->Alloc_Packet(SessionID);\n")
    f_w_cpp.write("\tHEADER header;\n")
    
    """
    
    f_w_cpp.write("\theader.length = ")
    
    data_type.pop(0)
    
    while len(data_type) != 0:
        
        f_w_cpp.write("sizeof(")
        f_w_cpp.write(data_type.pop(0))
        f_w_cpp.write(")")
        if(len(data_type) != 0)  :
             f_w_cpp.write("+")
        else:
             f_w_cpp.write(";\n")
    
     """
    
   

   

    queue.pop(0) 

    f_w_cpp.write("\tObj->Make_Header("+queue.pop(0)+", header);\n")
    
    f_w_cpp.write("\tpack->PutData((CHAR*)&header, sizeof(header));\n")
    
    f_w_cpp.write("\t*pack")
    
 
    
    while len(queue) != 0:
        
        f_w_cpp.write(" << ")
        f_w_cpp.write(queue.pop(0))
    f_w_cpp.write(";\n")
    
    # 변경의 여지가 있는 부분 
    f_w_cpp.write("\tObj->SendPacket(SessionID,pack);\n")
    f_w_cpp.write("}\n\n")
    
    
f_w_header.write("\n};")


f_w_header.close()
f_r.close()
f_w_cpp.close()

