using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShipPositionStatic : MonoBehaviour
{
    //�v���C���[�R���g���[���[
    private PlayerController controller;
    private GameObject player;

    public bool col = false;

    private void Start()
    {
        //�擾
        controller = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerController>();
        player = GameObject.FindGameObjectWithTag("Player");
    }

    //���������u��
    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.tag == "Ground")
        {
            col = true;
        }
    }
    //�n�ʂƔ�ڐG�ɂȂ����� 
    private void OnCollisionExit(Collision collision)
    {
        if (collision.gameObject.tag == "Ground")
        {
            col = false;
        }
    }

}
