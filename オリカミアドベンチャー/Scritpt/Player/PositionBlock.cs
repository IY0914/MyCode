using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PositionBlock : MonoBehaviour
{
    //��ɂȂ�I�u�W�F�N�g
    private GameObject homeObject;

    private void Start()
    {
        //��ɂȂ�I�u�W�F�N�g�̏��
        homeObject = GameObject.Find("PlayerStateCot");
    }
    // Update is called once per frame
    void Update()
    {
        //�����1.5cm��ɒu��
        this.transform.position = homeObject.transform.position + (Vector3.up * 1.5f);
    }
}
