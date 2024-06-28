using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlaneMove : MonoBehaviour
{
    //‘¬“x
    private float flyPower = 0.01f;
    //Ž‘±ŽžŠÔ
    private float useTime = 3.0f;
    private float FlatBomb = 0;
    //”š’e
    public GameObject bomb;

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        //Žg—pŽžŠÔ‚ð‰ÁŽZ
        useTime -= Time.deltaTime;
        FlatBomb += Time.deltaTime;

        //‰¡Ž²•ûŒü‚É—Í‚ð‰Á‚¦‚é
        this.transform.position += Vector3.right * flyPower;
        //2•b‚É‚P‰ñ”š’e‚ðU‚ç‚¹‚é
        if(FlatBomb > 1.0f)
        {
            Vector3 pos = transform.position + new Vector3(0.0f,-1.0f,0.0f); 

            GameObject.Instantiate(bomb,pos,this.transform.rotation) ;

            FlatBomb = 0;
        }
        //”jŠü
        if(useTime < 0.0f )
        {
            Destroy(this.gameObject);
        }
    }
}
