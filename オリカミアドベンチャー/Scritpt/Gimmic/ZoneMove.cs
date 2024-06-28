using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class ZoneMove : MonoBehaviour
{
  
    //�]�[���̎擾
    public GameObject arriveZone;
    //������΂��Ώۏ��
    private GameObject? targetObject = null;

    //������΂�����
    private bool thorow = false;

    /// <summary>
    /// �ˏo�p�x
    /// </summary>
    [SerializeField, Range(0F, 90F), Tooltip("�ˏo����p�x")]
    private float ThrowingAngle;

   
    private void Start()
    {
        //�v���C���[�̃I�u�W�F�N�g�擾
        targetObject = GameObject.FindGameObjectWithTag("Player");
        
    }

    private void Update()
    {
       

        //������΂����肪ON�ɂȂ�����
        if (thorow)
        {
            ThrowObject();
        }

        if (thorow) thorow = false;
    }

    /// <summary>
    /// ������I�u�W�F�N�g���ˏo����
    /// </summary>
    private void ThrowObject()
    {
        if (targetObject != null && arriveZone != null)
        {

            // �W�I�̍��W
            Vector3 targetPosition = arriveZone.transform.position;

            // �ˏo�p�x
            float angle = ThrowingAngle;

            // �ˏo���x���Z�o
            Vector3 velocity = CalculateVelocity(this.transform.position, targetPosition, angle);

            // �ˏo
            Rigidbody rid = targetObject.GetComponent<Rigidbody>();
            rid.AddForce(velocity * rid.mass, ForceMode.Impulse);
        }
        else
        {
            throw new System.Exception("�ˏo����I�u�W�F�N�g�܂��͕W�I�̃I�u�W�F�N�g�����ݒ�ł��B");
        }
    }

    /// <summary>
    /// �W�I�ɖ�������ˏo���x�̌v�Z
    /// </summary>
    /// <param name="pointA">�ˏo�J�n���W</param>
    /// <param name="pointB">�W�I�̍��W</param>
    /// <returns>�ˏo���x</returns>
    private Vector3 CalculateVelocity(Vector3 pointA, Vector3 pointB, float angle)
    {
        // �ˏo�p�����W�A���ɕϊ�
        float rad = angle * Mathf.PI / 180;

        // ���������̋���x
        float x = Vector2.Distance(new Vector2(pointA.x, pointA.z), new Vector2(pointB.x, pointB.z));

        // ���������̋���y
        float y = pointA.y - pointB.y;

        // �Ε����˂̌����������x�ɂ��ĉ���
        float speed = Mathf.Sqrt(-Physics.gravity.y * Mathf.Pow(x, 2) / (2 * Mathf.Pow(Mathf.Cos(rad), 2) * (x * Mathf.Tan(rad) + y)));

        if (float.IsNaN(speed))
        {
            // �����𖞂����������Z�o�ł��Ȃ����Vector3.zero��Ԃ�
            return Vector3.zero;
        }
        else
        {
            return (new Vector3(pointB.x - pointA.x, x * Mathf.Tan(rad), pointB.z - pointA.z).normalized * speed);
        }
    }

    /*
   private void Update()
 {
     //����ւ���̈ʒu���
     Vector3 pos = arriveZone.transform.position;

     //����ւ�������
     if (change_zone)
     {
         targetObject.transform.position = new Vector3(pos.x,pos.y,pos.z);
     }

     //���Z�b�g
     change_zone = false;
 }
     */

    private void OnTriggerEnter(Collider other)
    {
        if(other.gameObject.tag == "Player")
        {
             //����ւ�����
             thorow = true;

            //�����ԉ���
            other.gameObject.GetComponent<PlayerController>().Controlller = false;
            //�O���C�_�[��ԉ���
            other.gameObject.GetComponent<PlayerController>().glider = false;
            //�����x��
            other.gameObject.GetComponent<Rigidbody>().velocity = Vector3.zero;
            //���C�͂����Z�b�g
            other.gameObject.GetComponent<Rigidbody>().drag = 0.0f;

        }

    }
}
