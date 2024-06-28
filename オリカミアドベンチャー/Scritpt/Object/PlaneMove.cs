using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlaneMove : MonoBehaviour
{
    //���x
    private float flyPower = 0.01f;
    //��������
    private float useTime = 3.0f;
    private float FlatBomb = 0;
    //���e
    public GameObject bomb;

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        //�g�p���Ԃ����Z
        useTime -= Time.deltaTime;
        FlatBomb += Time.deltaTime;

        //���������ɗ͂�������
        this.transform.position += Vector3.right * flyPower;
        //2�b�ɂP�񔚒e��U�点��
        if(FlatBomb > 1.0f)
        {
            Vector3 pos = transform.position + new Vector3(0.0f,-1.0f,0.0f); 

            GameObject.Instantiate(bomb,pos,this.transform.rotation) ;

            FlatBomb = 0;
        }
        //�j��
        if(useTime < 0.0f )
        {
            Destroy(this.gameObject);
        }
    }
}
