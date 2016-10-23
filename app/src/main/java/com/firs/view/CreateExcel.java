package com.firs.view;

import java.io.File;

import jxl.Workbook;
import jxl.write.Label;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
import android.os.Environment;

public class CreateExcel {
	// ׼������excel������ı���
	private WritableSheet sheet;
	/**����Excel������*/
	private WritableWorkbook wwb;
	private String[] title = { "����", "����", "״̬", "��ʶ��"};

	public CreateExcel() {
		excelCreate();
	}

	public void excelCreate() {
		try {
			/**�����excel�ļ���·��*/
			String filePath = Environment.getExternalStorageDirectory() + "/RunVision";
			File file = new File(filePath, "worlk.xls");
			if (!file.exists()) {
				file.createNewFile();
			}
			wwb = Workbook.createWorkbook(file);
			/**��ӵ�һ�����������õ�һ��Sheet������*/
			sheet = wwb.createSheet("����ʶ��ǼǱ�", 0);
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void saveDataToExcel(int index, String[] content) throws Exception {
		Label label;
		for (int i = 0; i < title.length; i++) {
			/**Label(x,y,z)����x����Ԫ��ĵ�x+1�У���y+1��, ��Ԫ���������y
			 * ��Label������Ӷ�����ָ����Ԫ���λ�ú�����
			 * */
			label = new Label(i, 0, title[i]);
			/**������õĵ�Ԫ����ӵ���������*/
			sheet.addCell(label);
		}
		/*
		 * ��������䵽��Ԫ����
		 * ��Ҫʹ��jxl.write.Number
		 * ·������ʹ��������·�����������ִ���
		 */
		for (int i = 0; i < title.length; i++) {
			Label labeli = new Label(i, index, content[i]);
			sheet.addCell(labeli);
		}
		// д������
		wwb.write();
		// �ر��ļ�
		wwb.close();
	}

}
