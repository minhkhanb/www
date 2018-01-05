import java.nio.channels.FileChannel;
import java.nio.ByteBuffer;
import java.io.*;
import java.util.*;
import java.lang.*;
import jxl.*;


public class DataManifeshExport
{
	static private class ResourceItem{
		public String Define;
		public String FileName;
		public ResourceItem(String define, String fname){
			Define = define;
			FileName = fname;
		}
	}
	final static String k_sVersion		= "1.0.0";
	

	public static char GetMaskChar(char ch)
	{
		if(ch == '’')
		{
			ch = '\'';
		}
		if(ch == '“' || ch =='”')
		{
			ch = '"';
		}
		
		if(ch >= '!' && ch <= '~')
		
		{
			return ch;
		}
		return ' ';
	}
	public static void PrintLicence(PrintStream out)
	{
			out.println("//Data Manifesh exporter");
			out.println("//Programmer: Do Thanh Nghia");
	}
	public static void main (String[] args)
	{
		try
		{
			File f = new File(args[0]);
		
			Workbook workbook = Workbook.getWorkbook(f);
			
			Sheet[] sheets = workbook.getSheets();
		
			exportData(sheets[0]);
			exportSound(sheets[1]);
			exportText(sheets[2]);
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		
		
	}
	public static void exportData(Sheet sheet)
	{
		try
		{
			String file_name = "lib.manifest";
			FileOutputStream os = new FileOutputStream(file_name);
			PrintStream out = new PrintStream(os);
			
			
			//PrintLicence(out);
			//out.println("akjglaskjgalgj");
			Cell cell0[] = sheet.getRow(3);
			/* Lam Add new */
			int Main_Ver_Index = getIntVal(MaskString(cell0[0].getContents()));
			int total_ver = getIntVal(MaskString(cell0[1].getContents()));
			int num = getIntVal(MaskString(cell0[3].getContents()));
			String main_ver = MaskString(cell0[4].getContents());
			int num_ver = getIntVal(MaskString(cell0[5].getContents()));
			//System.out.println("444444444444444444444444444");
			String SpriteIDName[] = new String[num];
			String SpriteID_Version[][] = new String[total_ver][num];
			String FileName_Version[][] = new String[total_ver][num];
			String PackName_Version[][] = new String[total_ver][num];
			int ColumnIndex_Version[] = new int[total_ver];
			String NameDevice[] = new String[total_ver];
			
			String PackNameMain[] = new String[num];
			int NumPackMain = 0;
			/*** Get Ver Is Use	***/
			int Ver_Is_Use[] = new int[total_ver];
			for(int i = 0; i<total_ver;i++)
			{
				try
				{
					Cell cell[] = sheet.getRow(5+i);
					Ver_Is_Use[i] =getIntVal( MaskString(cell[0].getContents()));
				}
				catch(Exception e)
				{
					
				}
				
			}
			//out.println("main ver = "+Main_Ver_Index);
			/*** Get name versions use  ***/
			int count_real = 0;
			for(int i = 0; i<total_ver; i++)
			{
				try
				{
					Cell cell[] = sheet.getRow(5+i);
					NameDevice[i] = MaskString(cell[1].getContents());
					//out.println("name ver = "+NameDevice[i]);
				}
				catch(Exception e)
				{
					
				}
			}
			/*** Get column index of version   ***/
			count_real = 0;
			for(int i = 0; i<total_ver; i++)
			{
				try
				{
					Cell cell[] = sheet.getRow(5+i);
					ColumnIndex_Version[i] = getIntVal(MaskString(cell[2].getContents()));
				}
				catch(Exception e)
				{
					
				}	
			}
			/*** Get SpriteID  ***/
			for(int i = 0; i< num;i++)
			{
				try
				{
					Cell cell[] = sheet.getRow(5+i);
					SpriteIDName[i] = MaskString(cell[4].getContents());
				}
				catch(Exception e)
				{
					
				}
				PackNameMain[i] = "0";
			}
			/***  get data main version ***/
			for(int j = 0; j<num; j++)
			{
				try
				{
					Cell cell[] = sheet.getRow(5+j);
					SpriteID_Version[Main_Ver_Index][j] = MaskString(cell[ColumnIndex_Version[Main_Ver_Index]].getContents());
					FileName_Version[Main_Ver_Index][j] = MaskString(cell[ColumnIndex_Version[Main_Ver_Index]+1].getContents());
					PackName_Version[Main_Ver_Index][j] = MaskString(cell[ColumnIndex_Version[Main_Ver_Index]+2].getContents());
				}
				catch(Exception e)
				{
					
				}
			}
			PackNameMain[0] = PackName_Version[Main_Ver_Index][0];
			NumPackMain = 1;
			/****** Get Pack Name to PackName Main ******/
			int count_of_pack[] = new int[num];
			for(int i = 1; i< num;i++)
			{
				//count_of_pack[NumPackMain-1] = 1;
				String pack_temp = PackName_Version[Main_Ver_Index][i];
				boolean new_pack = true;
				for(int j = 0; j<NumPackMain;j++)
				{
					if(pack_temp.compareToIgnoreCase(PackNameMain[j])== 0)
					{					
						count_of_pack[j]++;
						new_pack = false;
					}
				}
				if(new_pack == true)
				{
					PackNameMain[NumPackMain] = pack_temp;
					NumPackMain++;
				}
			}
			String file_sprite_name = "sprite_namedata.sprite";
			FileOutputStream os_sprite_name = new FileOutputStream(file_sprite_name);
			PrintStream out_sprite_name = new PrintStream(os_sprite_name);
			/*****	Print ******/
			int count_pack = 0;
			String file_name_version = "Ver_"+NameDevice[Main_Ver_Index];
			FileOutputStream os_name_version = new FileOutputStream(file_name_version);
			PrintStream out_name_version = new PrintStream(os_name_version);
			for(int i = 0; i<NumPackMain;i++)
			{
				out.println("LIBNAME \t\""+PackNameMain[i] + "_" + NameDevice[Main_Ver_Index]+"\"\t\t\t" +PackNameMain[i].toUpperCase()+"_"+NameDevice[Main_Ver_Index]+"_PACK");
				out_name_version.println(PackNameMain[i] + "_" + NameDevice[Main_Ver_Index]);
				int count = 0;
				for(int j = 0; j<num;j++)
				{
					if(PackName_Version[Main_Ver_Index][j].compareToIgnoreCase(PackNameMain[i]) == 0)
					{
						out.println("\tADD \t\t\".\\"+ FileName_Version[Main_Ver_Index][j] +".bpine\"\t\t\t\t\t"+SpriteID_Version[Main_Ver_Index][j]);
						count++;
						out_sprite_name.println(FileName_Version[Main_Ver_Index][j] +".pine");
					}
					if(count == count_of_pack[i]+1)
					{
						out.println("ENDLIB");
						break;
					}
				}
			}
			out_name_version.close();
			/*--------------- End -----------------*/
			/******		Get data for version other 	******/ 
			for(int i = 0; i<total_ver; i++)
			{
				if(Ver_Is_Use[i] != 0)
				{
					for(int j = 0; j<num; j++)
					{
						try
						{
							Cell cell[] = sheet.getRow(5+j);
							SpriteID_Version[i][j] = MaskString(cell[ColumnIndex_Version[i]].getContents());
							FileName_Version[i][j] = MaskString(cell[ColumnIndex_Version[i]+1].getContents());
							PackName_Version[i][j] = MaskString(cell[ColumnIndex_Version[i]+2].getContents());
						}
						catch(Exception e)
						{
							
						}
					}
				}
			}
				//count_pack = 0;
				String PackNameReUse[] = new String[total_ver];
				String PackNameCurrent[] = new String[total_ver];
				int pack_name_num_off = 0;
			for(int i = 0; i<total_ver;i++)
			{
				//out.println("i = " +i);
				if(Ver_Is_Use[i] != 0 && i != Main_Ver_Index)
				{
					String file_name_ver = "Ver_"+NameDevice[i];
					FileOutputStream os_name_ver = new FileOutputStream(file_name_ver);
					PrintStream out_name_ver = new PrintStream(os_name_ver);
					//out.println("num pack = " +NumPackMain);
					for(int k = 0; k< NumPackMain; k++)
					{
						boolean is_print = true;
						boolean not_pack = true;
						int num_of_pack = 0;
						boolean is_print_main = true;
						boolean is_reuse = true;
						for(int l = 0; l<num;l++)
						{
							//out.println(" num id = "+l);
							//out.println(" pack name = "+PackName_Version[i][l]);
							if(PackName_Version[i][l].compareToIgnoreCase(PackNameMain[k]) == 0 && PackName_Version[i][l].compareToIgnoreCase("0")!=0)
							{
								not_pack = false;
								if(is_print == true)
								{
									out.println("LIBNAME \t\""+PackNameMain[k] + "_" +NameDevice[i]+"\"\t\t\t" +PackNameMain[k].toUpperCase()+"_"+NameDevice[i]+"_PACK");
									out_name_ver.println(PackNameMain[k] + "_" +NameDevice[i]);
									is_print = false;
								}
								out_sprite_name.println(FileName_Version[i][l] +".pine");
								out.println("\tADD \t\t\".\\"+ FileName_Version[i][l] +".bpine\"\t\t\t\t\t"+SpriteID_Version[i][l]);
								num_of_pack++;							
							}
							if(PackName_Version[i][l].compareToIgnoreCase("0") == 0 && PackName_Version[Main_Ver_Index][l].compareToIgnoreCase(PackNameMain[k])==0 && is_print_main == true)
							{
								out_name_ver.println(PackNameMain[k] + "_" +NameDevice[Main_Ver_Index]);
								is_print_main = false;
								not_pack = false;
							}
							if(PackName_Version[i][l].compareToIgnoreCase(PackNameMain[k]) != 0 && PackName_Version[i][l].compareToIgnoreCase("0")!=0)
							{
								not_pack = false;
							}
							if(l == num-1 && is_print == false)
							{
								out.println("ENDLIB");
							}
						}
						if(not_pack == true)
						{
							out_name_ver.println(PackNameMain[k] + "_" +NameDevice[Main_Ver_Index]);
						}
					}
					/*** Get pack name reuse ***/
					for(int d = 0; d<NumPackMain;d++)
					{
						boolean is_print = true;
						for(int l = 0; l<num;l++)
						{
							//System.out.println("PackName_Version[i][l] = "+PackName_Version[i][l]);
							if( PackName_Version[i][l].compareToIgnoreCase(PackNameMain[d]) != 0 && PackName_Version[i][l].compareToIgnoreCase("0") != 0 && PackName_Version[Main_Ver_Index][l].compareToIgnoreCase(PackNameMain[d])==0)
							{
								for(int k = 0; k<total_ver;k++)
								{
									if(PackName_Version[i][l].compareToIgnoreCase(NameDevice[k]) == 0)
									{
										if(is_print == true)
										{
											out_name_ver.println(PackNameMain[d] + "_" +NameDevice[k]);
											is_print = false;
										}
									}
								}
							}
						}
					}
					out_name_ver.close();
					
				}
			}
			//out.close();
			os.close();
			out_sprite_name.close();
			//Export to sprite.h
			String file_sprite = "SpriteList.h";
			FileOutputStream os_sprite = new FileOutputStream(file_sprite);
			PrintStream out_sprite = new PrintStream(os_sprite);
			out_sprite.println("#if !_SPRITELIST_H");
			out_sprite.println("#define _SPRITELIST_H 1");
			out_sprite.println();
			int s = num+50;
			for(int i =0;i < num;i++)
			{
				out_sprite.println("#define \t" + SpriteIDName[i] + "\t\t" +i);
			}
			out_sprite.println();
			out_sprite.println("#define \t TOTAL_SPRITE_INGAME \t"+ s);
			out_sprite.println("#endif");
			
			out_sprite.close();
			// Export to SpriteList.cpp
			String file_spritelib = "lib_SpriteList.cpp";
			FileOutputStream os_sp = new FileOutputStream(file_spritelib);
			PrintStream out_sp = new PrintStream(os_sp);
			out_sp.println("const int CGame::_syn_sprite_index[VERSION_NUMS*TOTAL_SPRITE_INGAME]=");
			out_sp.println("{");
			
			for(int i = 0; i< num; i++)
			{
				out_sp.print("\n/*"+SpriteIDName[i]+"*/");
				//System.out.println("iiiiii = "+i);
				for(int j = 0; j< total_ver; j++)
				{
					
					if(Ver_Is_Use[j] != 0)
					{
						if((PackName_Version[j][i].compareToIgnoreCase("0")) == 0)
						{
							out_sp.print("\t\t"+SpriteID_Version[Main_Ver_Index][i]+",");
							//System.out.println("111 Ver_Is_Use["+j+"] = "+Ver_Is_Use[j]);
						}
						else
						{
							boolean reuse = true;
							for(int k = 0; k<NumPackMain;k++)
							{
								if(PackName_Version[j][i].compareToIgnoreCase(PackNameMain[k]) == 0)
								{
									out_sp.print("\t\t"+SpriteID_Version[j][i]+",");
									reuse = false;
								}
							}
							if(reuse == true)
							{
								for(int k = 0;k<total_ver;k++)
								{
									if(PackName_Version[j][i].compareToIgnoreCase(NameDevice[k]) == 0)
									{
										out_sp.print("\t\t"+SpriteID_Version[k][i]+",");
									}
								}
							}
							//System.out.println("222 Ver_Is_Use["+j+"] = "+Ver_Is_Use[j]);
						}
					}
					else
					{
						out_sp.print("\t\t"+SpriteID_Version[Main_Ver_Index][i]+",");
						//System.out.println("SpriteID_Version[Main_Ver_Index][i] = "+SpriteID_Version[Main_Ver_Index][i]);
						//System.out.println("333 Ver_Is_Use["+j+"] = "+Ver_Is_Use[j]);
					}
				}
			}
			out_sp.println("\n};");
			/*------------------ End -------------------*/
		}
		catch(Exception e)
		{
		}	
	}
	public static void exportSound(Sheet sheet)
	{
			System.out.println("export sound");
		try
		{
			String file = "lib.manifest";
			FileOutputStream os_sound = new FileOutputStream(file,true);
			PrintStream out_sound = new PrintStream(os_sound);

			
			Cell cell0[] = sheet.getRow(3);
			int num = getIntVal(MaskString(cell0[0].getContents()));
			String Sound_ID[] = new String[num];
			String File_Name[] = new String[num];
			String Status[] = new String[num];
			for(int i = 0; i< num;i++)
			{
				try
				{
					Cell cell[] = sheet.getRow(5+i);
					Sound_ID[i] = MaskString(cell[0].getContents());
					File_Name[i] = MaskString(cell[1].getContents());
					Status[i] = MaskString(cell[2].getContents());
				}
				catch(Exception e)
				{
				}
			}
			ArrayList sound_list = new ArrayList();
			ArrayList music_list = new ArrayList();
			
			for(int i = 0; i < num ;i++)
			{
				String fname = Status[i];
				String define = File_Name[i].toUpperCase();
				
				int dot = fname.lastIndexOf('.');
				if(dot > 0){
					String extension = fname.substring(dot+1);
					if(extension.compareTo("mp3") == 0){
						music_list.add(new ResourceItem(define, fname));
						System.out.println("add music:" + fname);
					}else{
						sound_list.add(new ResourceItem(define, fname));
					}
				}else{
					sound_list.add(new ResourceItem(define, fname));
				}
			}

			out_sound.println("\nLIBNAME\t\t\"sfx\"\t\t\tSFX_PACK");
			for(int s = 0; s< sound_list.size(); s++){
				ResourceItem item = (ResourceItem)sound_list.get(s);
				out_sound.println("\tADD\t\t\".\\"+item.FileName+"\"\t\t\t"+item.Define);
			}
			out_sound.println("ENDLIB");
			
			for(int m = 0; m< music_list.size(); m++){
				ResourceItem item = (ResourceItem)music_list.get(m);
				out_sound.println("\nLIBNAME\t\t\""+ (item.Define).toLowerCase() +"\"\t\t\t" + item.Define + "_PACK");
				out_sound.println("\tADD\t\t\".\\"+item.FileName+"\"\t\t\t"+item.Define);
				out_sound.println("ENDLIB");
			}
			out_sound.close();

		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	public static void exportText(Sheet sheet)
	{
		try
		{

			Cell cell0[] = sheet.getRow(3);
			int num = getIntVal(MaskString(cell0[0].getContents()));
			String num_text[] = new String[num];
			String text_ID[] = new String[num];
			for(int i = 0 ; i < num ; i++)
			{
				try
				{
					Cell cell[] = sheet.getRow(5+i);
					num_text[i] = MaskString(cell[0].getContents());
					text_ID[i] = MaskString(cell[1].getContents());
				}
				catch(Exception e)
				{
				}
			}	
			String file_t = "Text.h";
			FileOutputStream os_t = new FileOutputStream(file_t);
			Writer out_t = new OutputStreamWriter(os_t,"Unicode");
			for(int i = 0; i < num; i++)
			{
				out_t.write("\""+text_ID[i]+"\"\n");
			}
			out_t.close();
			String file_e = "E_Text.bin";
			FileOutputStream os_e = new FileOutputStream(file_e);
			DataOutputStream out_e = new DataOutputStream(os_e);
			out_e.writeShort(num);
			String Data_e = "";
			for(int i = 0; i < num; i++)
			{
				//out_e.writeChars(i+"\n");
				String str = text_ID[i];
				out_e.writeShort(Data_e.length());
				for(int j = 0; j<str.length();j++)
				{
					char ch = str.charAt(j);  
					Data_e += ch;
				}				
				out_e.writeShort(Data_e.length()-1);
			}
			for(int i = 0; i < num; i++)
			{
				String str = text_ID[i];
				for(int j = 0; j<str.length();j++)
				{
					short ch = (short)str.charAt(j); 
					out_e.writeShort(ch);	
				}
			}
			out_e.close();
		}
		catch (Exception e)
		{
		}
	}
	private static void WriteString(FileOutputStream os, String str)
	{
		for(int i = 0; i < str.length(); i++)
		{
			try
			{
				char ch = GetMaskChar(str.charAt(i));
				os.write((ch)&0xFF);
			}
			catch(Exception e)
			{
			System.out.printf("Error e"+e);
			}
				
		}
	}
	private static void WriteInt(FileOutputStream os, int i)
	{
		
		try
		{
			os.write((i>>24)&0xFF);
			os.write((i>>16)&0xFF);
			os.write((i>>8)&0xFF);
			os.write(i&0xFF);
		}
		catch (Exception ex)
		{
			
		}
	}
	private static void WriteFloat(FileOutputStream os, float f)
	{
		int i = Float.floatToIntBits(f);
		
		try
		{
			os.write((i>>24)&0xFF);
			os.write((i>>16)&0xFF);
			os.write((i>>8)&0xFF);
			os.write(i&0xFF);
		}
		catch (Exception ex)
		{
			
		}
	}
	static String MaskString(String strval)
	{
		return MaskString(strval,0);
	}
	static String MaskString(String strval,int value)
	{
		if(strval == null || strval == "" || strval.length() <= 0)
			return ""+value;
		return strval;
	}
	static int getIntVal(String strval)
	{
		if(strval == null || strval == "")
			return 0;
		strval = strval.trim().replace(",", "");
		return Integer.parseInt(strval);
	}
	static double getFloatVal(String strval)
	{
		if(strval == null || strval == "")
			return 0;
		strval = strval.trim().replace(",", "");
		return Double.parseDouble(strval);
	}
	static void Log (String s)
	{
		System.out.println(s);
	}
	
	static byte[] getBytes(String str)
	{
		try
		{
			return str.getBytes("UTF-8");
		}
		catch(Exception e)
		{
		}
		return str.getBytes();
	}
}
