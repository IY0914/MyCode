using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackMove : MonoBehaviour
{
    //�v���C���[�̏��
    private GameObject controller;
  

    private void Start()
    {
        //�擾�ꗗ
        controller = GameObject.FindGameObjectWithTag("Player");
      
    }
    // Update is called once per frame
    void Update()
    {
       
            //�v���C���[�̉��ړ��ʂ������擾
            this.transform.position = new Vector3(controller.transform.position.x, 6.0f, -100.0f);
        
    }
}
