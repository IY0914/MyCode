using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BombScript : MonoBehaviour
{
    //�p�[�e�B�N��
    public GameObject particle;
    //�}�e���A��
    MeshRenderer material;
    //�N������
    public float detonationTime = 3.0f;
    //�F����
    public float timeR = 0.0f;

    private void Start()
    {
        material = GetComponent<MeshRenderer>();
    }

    private void Update()
    {
        //���ԉ��Z
        detonationTime -= Time.deltaTime;
        //3�b�ŋN������
        if(detonationTime < 0.0f)
        {
            Detonaton();
        }

        timeR =  (255.0f / 3.0f) * Time.deltaTime;

        //�N�����Ԃ�F�ł킩��悤�ɂ���s
        material.material.color = material.material.color + new Color32((byte)(timeR),0,0,0);

    }

    //�N���֐�
    void Detonaton()
    {
        //����
        Destroy(this.gameObject);
        //�p�[�e�B�N���I�u�W�F�N�g�̐���
        Instantiate(particle, this.transform.position, this.transform.rotation);
    }

    private void OnCollisionEnter(Collision collision)
    {
        //�G�ɓ��������ꍇ
        if (collision.gameObject.tag == "Enemy")
        {
            Detonaton();
        }
    }
}
